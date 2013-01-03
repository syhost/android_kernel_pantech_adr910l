//=============================================================================
// File       : dmb_tsif.c
//
// Description: 
//
// Revision History:
//
// Version         Date           Author        Description of Changes
//-----------------------------------------------------------------------------
//  1.0.0       2010/12/06       yschoi         Create
//  1.1.0       2011/09/29       yschoi         tdmb_tsif.c => dmb_tsif.c
//=============================================================================

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/i2c.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

#include "dmb_comdef.h"
#include "dmb_tsif.h"
#include "dmb_i2c.h"
#include "dmb_type.h"
#include "dmb_test.h"

#ifdef CONFIG_SKY_TDMB
#include "tdmb/tdmb_chip.h"
#include "tdmb/tdmb_bb.h"
#include "tdmb/tdmb_test.h"

#endif

#ifdef CONFIG_SKY_ISDBT
#include "isdbt/isdbt_bb.h"
#include "isdbt/isdbt_chip.h"
#include "isdbt/isdbt_test.h"

#endif

#ifdef FEATURE_DMB_TSIF_CLK_CTL
#include <../../arch/arm/mach-msm/clock-pcom.h>
#endif



/*================================================================== */
/*==============        DMB TSIF Driver Definition     =============== */
/*================================================================== */

extern void tsif_force_stop(void);
extern void tsif_test_dmb(void);

#ifdef CONFIG_SKY_TDMB
extern ts_data_type ts_data;
extern tdmb_mode_type dmb_mode;
#endif

#ifdef CONFIG_SKY_ISDBT
extern isdbt_ts_data_type ts_data;
#endif



/*================================================================== */
/*==============        DMB TSIF Driver Function     =============== */
/*================================================================== */

#ifdef CONFIG_SKY_TDMB
/*====================================================================
FUNCTION       tdmb_tsif_data_parser  
DESCRIPTION 
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
======================================================================*/
static void tdmb_tsif_data_parser(char* user_buf, void * data_buffer, int size)
{
  int i, packet_size;
#if defined(FEATURE_TDMB_USE_RTV_MTV350) && defined(FEATURE_TDMB_MULTI_CHANNEL_ENABLE)
  static char temp_ts_buf[BB_MAX_DATA_SIZE];
  RTV_CIF_DEC_INFO cif_dec_info;
  //ts_data_type *user_buf = (ts_data_type *)buffer;
#elif defined(FEATURE_TDMB_USE_TCC_TCC3170) && defined(FEATURE_TDMB_MULTI_CHANNEL_ENABLE)
  static char temp_ts_buf[BB_MAX_DATA_SIZE];
#endif

  //DMB_MSG_TSIF ("[%s]\n", __func__);

  if(data_buffer == NULL || size <= 0)
  {
    DMB_MSG_TSIF("[%s] TSIF data buffer [%x] Or Size [%d]",__func__, (int)data_buffer, size);
    ts_data.type = TYPE_NONE;
    ts_data.fic_size = 0;
    ts_data.msc_size = 0;

    if (copy_to_user((void __user *)user_buf, &ts_data, sizeof(ts_data)))
    {
      return;
    }

    return;
  }

  packet_size = (size / TSIF_DATA_SIZE) * TDMB_TS_PKT_SIZE;

  if (packet_size > BB_MAX_DATA_SIZE)
  {
    DMB_MSG_TSIF("[%s] overflow! size[%d], packet_size[%d].cnt[%d]\n", __func__, size, packet_size, packet_size/TDMB_TS_PKT_SIZE);
    DMB_MSG_TSIF("[%s] Max data buf[%d], Max TSIF buf[%d]\n", __func__, BB_MAX_DATA_SIZE, TSIF_DATA_SIZE*TSIF_CHUNK_SIZE);
    packet_size = BB_MAX_DATA_SIZE;
  }

  for(i=0; i < (packet_size/TDMB_TS_PKT_SIZE); i++)
  {
#if (defined(FEATURE_TDMB_USE_RTV_MTV350) || defined(FEATURE_TDMB_USE_TCC_TCC3170)) && defined(FEATURE_TDMB_MULTI_CHANNEL_ENABLE)
    memcpy((void*)&temp_ts_buf[i*TDMB_TS_PKT_SIZE], (data_buffer+i*TSIF_DATA_SIZE), TDMB_TS_PKT_SIZE);
#else
    memcpy((void*)&ts_data.msc_buf[i*TDMB_TS_PKT_SIZE], (data_buffer+i*TSIF_DATA_SIZE), TDMB_TS_PKT_SIZE);
    //TDMB_MSG_TCC_BB("%s [%x] [%x] [%x] [%x]\n", __func__, ts_data.msc_buf[i*TDMB_TS_PKT_SIZE],ts_data.msc_buf[i*TDMB_TS_PKT_SIZE+1], ts_data.msc_buf[i*TDMB_TS_PKT_SIZE+2], ts_data.msc_buf[i*TDMB_TS_PKT_SIZE+3]);
#endif
  }

#ifdef FEATURE_TDMB_MULTI_CHANNEL_ENABLE
  ts_data.type = TYPE_NONE;
  ts_data.fic_size = 0;
#if defined(FEATURE_TDMB_USE_INC_T3900)
  ts_data.msc_size = t3700_header_parsing_tsif_data(ts_data.msc_buf, packet_size);
#elif defined(FEATURE_TDMB_USE_FCI_FC8050)
  fc8050_demux(ts_data.msc_buf, packet_size);
#elif defined(FEATURE_TDMB_USE_RTV_MTV350)
  //cif_dec_info.fic_size = &ts_data.fic_size; //user_buf->fic_size;
  //cif_dec_info.msc_size[0] = &ts_data.msc_size; //user_buf->msc_size;
  cif_dec_info.fic_buf_ptr = ts_data.fic_buf;//user_buf->fic_buf;
  cif_dec_info.msc_buf_ptr[0] = ts_data.msc_buf;   //user_buf->msc_buf
  
  rtvCIFDEC_Decode(&cif_dec_info, temp_ts_buf, packet_size);

  ts_data.fic_size = cif_dec_info.fic_size;
  ts_data.msc_size = cif_dec_info.msc_size[0];

  if((ts_data.msc_size==0) && (ts_data.fic_size == 0))
  {
    DMB_MSG_TSIF("mtv350 tsi parsing error!!! !packet_size=[%d]\n", packet_size);
    ts_data.type = TYPE_NONE; // copy_to_user(user_buf->type, &aaa. sizeof(user_buf->type));
    ts_data.fic_size = 0;
    ts_data.msc_size = 0;
  }
  else
  {
    if(ts_data.msc_size)
    {
      ts_data.type = MSC_DATA;
      if(ts_data.fic_size)
        ts_data.type = FIC_MSC_DATA;
    }
    else if(ts_data.fic_size)
    {
      ts_data.type = FIC_DATA;
    }
    //DMB_MSG_TSIF ("msc_buf=[0x%2x 0x%2x 0x%2x 0x%2x], msc_size=[%4d], fic_size=[%3d], packet_size=[%4d]", 
    //                             ts_data.msc_buf[0], ts_data.msc_buf[1], ts_data.msc_buf[2], ts_data.msc_buf[3], ts_data.msc_size, ts_data.fic_size, packet_size);
  }
#elif defined(FEATURE_TDMB_USE_TCC_TCC3170)
  ts_data.type = MSC_DATA;
  ts_data.fic_size = 0;
  //DMB_MSG_TSIF ("[%s] msc_size[%d]\n", __func__, packet_size);
  //dmb_data_dump(ts_data.msc_buf, packet_size, "/data/misc/dmb/raw_msc.txt");
  tcc3170_parse_data(temp_ts_buf, packet_size);
#else
  ##error
#endif /* FEATURE_TDMB_USE_INC_T3900 */
#else // Single Channel
  ts_data.type = MSC_DATA;
  ts_data.fic_size = 0;
  ts_data.msc_size = packet_size;
#endif /* FEATURE_TDMB_MULTI_CHANNEL_ENABLE */

  //DMB_MSG_TSIF ("[%s] fic_size[%d], msc_size[%d]\n", __func__, ts_data.fic_size, ts_data.msc_size);

#ifdef FEATURE_DMB_TSIF_READ_ONCE
  if (copy_to_user((void __user *)user_buf, &ts_data, sizeof(ts_data)))
  {
    return;
  }
#endif /* FEATURE_DMB_TSIF_READ_ONCE */

  if (dmb_mode == TDMB_MODE_NETBER)
  {
    netber_GetError(ts_data.msc_size, ts_data.msc_buf);
  }

  //DMB_MSG_TSIF ("[%s] end\n", __func__);
}
#endif /* CONFIG_SKY_TDMB */


#ifdef CONFIG_SKY_ISDBT
/*====================================================================
FUNCTION       isdbt_tsif_data_parser  
DESCRIPTION 
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
======================================================================*/
static void isdbt_tsif_data_parser(char* user_buf, void * data_buffer, int size)
{
  int i, packet_size;
  
  //ISDBT_MSG_DEV ("[%s]\n", __func__);
  if(data_buffer == NULL || size <= 0)
  {
    ts_data.ts_size = 0;
    DMB_MSG_TSIF("[%s] TSIF data buffer [%x] Or Size [%d]",__func__, (int)data_buffer, size);
    if (copy_to_user((void __user *)user_buf, &ts_data, sizeof(ts_data)))
    {
      return;
    }
    return;
  }
  
  packet_size = (size / TSIF_DATA_SIZE) * TDMB_TS_PKT_SIZE;
  
  if (packet_size > BB_MAX_DATA_SIZE)
  {
    DMB_MSG_TSIF("[%s] Max data buf[%d], Max TSIF buf[%d]\n", __func__, BB_MAX_DATA_SIZE, TSIF_DATA_SIZE*TSIF_CHUNK_SIZE);
    packet_size = BB_MAX_DATA_SIZE;
  }
  
  for(i=0; i < (packet_size/TDMB_TS_PKT_SIZE); i++)
  {
    memcpy((void*)&ts_data.ts_buf[i*TDMB_TS_PKT_SIZE], (data_buffer+i*TSIF_DATA_SIZE), TDMB_TS_PKT_SIZE);
  }
  
  ts_data.ts_size = packet_size;
  
  if (copy_to_user((void __user *)user_buf, &ts_data, sizeof(ts_data)))
  {
    return;
  }
  
  //ISDBT_MSG_DEV ("[%s] end\n", __func__);
}
#endif /* CONFIG_SKY_ISDBT */


/*===========================================================================
FUNCTION       dmb_tsif_data_parser
DESCRIPTION
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
===========================================================================*/
void dmb_tsif_data_parser(char* user_buf, void * data_buffer, int size)
{
#ifdef CONFIG_SKY_TDMB
  tdmb_tsif_data_parser(user_buf, data_buffer, size);
#endif

#ifdef CONFIG_SKY_ISDBT
  isdbt_tsif_data_parser(user_buf, data_buffer, size);
#endif
}
EXPORT_SYMBOL(dmb_tsif_data_parser);


/*===========================================================================
FUNCTION       dmb_tsif_force_stop
DESCRIPTION
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
===========================================================================*/
void dmb_tsif_force_stop(void)
{
  tsif_force_stop();
}


#if 0 //notused. def FEATURE_DMB_TSIF_CLK_CTL
/*===========================================================================
FUNCTION       dmb_tsif_clk_enable
DESCRIPTION
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
===========================================================================*/
void dmb_tsif_clk_enable(void)
{
  int rc;

  DMB_MSG_TSIF("[%s]\n", __func__);
  
  rc = pc_clk_enable(P_TSIF_CLK);
  if(rc)
  {
    DMB_MSG_TSIF("[%s] pc_clk_enable fail!!! [%d]\n", __func__, rc);
  }
}


/*===========================================================================
FUNCTION       dmb_tsif_clk_disable
DESCRIPTION
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
===========================================================================*/
void dmb_tsif_clk_disable(void)
{
  DMB_MSG_TSIF("[%s]\n", __func__);

  pc_clk_disable(P_TSIF_CLK);
}
#endif /* FEATURE_DMB_TSIF_CLK_CTL */


/*===========================================================================
FUNCTION       dmb_tsif_test
DESCRIPTION
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
===========================================================================*/
void dmb_tsif_test(void)
{
  DMB_MSG_TSIF("[%s]\n", __func__);

  tsif_test_dmb();
}

