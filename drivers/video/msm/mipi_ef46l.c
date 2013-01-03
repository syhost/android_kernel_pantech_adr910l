/* Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_rohm.h"
#include <mach/gpio.h>
#include <asm/irq.h>
#include <asm/system.h>

#define GPIO_HIGH_VALUE 1
#define GPIO_LOW_VALUE  0

#define NOP()	do {asm volatile ("NOP");} while(0);
#define DELAY_3NS() do { \
    asm volatile ("NOP"); \
    asm volatile ("NOP"); \
    asm volatile ("NOP");} while(0);

#define LCD_DEBUG_MSG

#ifdef LCD_DEBUG_MSG
#define ENTER_FUNC()        printk(KERN_INFO "[SKY_LCD] +%s \n", __FUNCTION__);
#define EXIT_FUNC()         printk(KERN_INFO "[SKY_LCD] -%s \n", __FUNCTION__);
#define ENTER_FUNC2()       printk(KERN_ERR "[SKY_LCD] +%s\n", __FUNCTION__);
#define EXIT_FUNC2()        printk(KERN_ERR "[SKY_LCD] -%s\n", __FUNCTION__);
#define PRINT(fmt, args...) printk(KERN_INFO fmt, ##args)
#define DEBUG_EN 1
#else
#define PRINT(fmt, args...)
#define ENTER_FUNC2()
#define EXIT_FUNC2()
#define ENTER_FUNC()
#define EXIT_FUNC()
#define DEBUG_EN 0
#endif

#if (BOARD_VER >= TP10)
#define FEATURE_TP_SAMPLE
#elif (BOARD_VER == WS20)
//#define FEATURE_WS_SAMPLE
#endif

//#define FEATURE_ROHM_ID_READ

extern int gpio43, gpio16, gpio24; /* gpio43 :reset, gpio16:lcd bl en , gpio24:lcd_bl_ctrl */

#ifndef FEATURE_ROHM_CABC_ON
#define FEATURE_SKY_BACKLIGHT_TPS61161
#endif

#ifdef FEATURE_SKY_BACKLIGHT_TPS61161
//#define LCD_BL_EN        70    //gpio24 
#define BL_MAX           31    //BL LEVEL

#define T_LOW_LB          8    //LOGIC 0  (T_HIGH_LB*2)
#define T_HIGH_LB         4    //LOGIC 0  (MIN 2us ~ 360us) 
#define T_LOW_HB          4    //LOGIC 1  (MIN 2us ~ 360us) 
#define T_HIGH_HB         8    //LOGIC 1  (T_LOW_HB*2)
#define T_START           4    //2us
#define T_EOS             4    //2us
#define T_ES_DELAY      200    //100us
#define T_ES_DETECT     500    //260us
#define T_ES_WIN          1    //1ms
#define T_SHUTDOWN        5    //5ms 
#define NUM_ADDR_DIGIT    8    //0x72 
#define NUM_DATA_DIGIT    8    //RFA(7)ADD1(6)ADD0(5)DATA(4:1)
#define DEVICE_ADDR    0x72
#endif

static struct msm_panel_common_pdata *mipi_rohm_pdata;

static struct dsi_buf rohm_tx_buf;
static struct dsi_buf rohm_rx_buf;

struct lcd_state_type {
    boolean disp_powered_up;
    boolean disp_initialized;
    boolean disp_on;
};

static struct lcd_state_type rohm_state = { 0, };

#if defined (FEATURE_TP_SAMPLE)
char extcctl[6]     = {0xdf, 0x55,0xaa,0x52,0x08,0x10};
char mtp_1[2]       = {0xf1, 0x08};
char mtp_2[4]      = {0xf7, 0x25,0x00,0x3d};
#elif defined (FEATURE_WS_SAMPLE)
char extcctl[5]     = {0xdf, 0x55,0xaa,0x52,0x08};
char eics[9]        = {0xb0, 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char mtp_1[2]       = {0x91, 0x84};
char mtp_2[2]       = {0x93, 0x03};
char mtp_3[2]       = {0xc0, 0x19};
char mtp_4[2]       = {0xc1, 0xc1};
char mtp_5[2]       = {0x90, 0x01};
char mtp_6[2]       = {0x93, 0x00};
char pw_lock[5]    = {0xdf, 0x55,0xaa,0x52,00};
#else
char extcctl[5]     = {0xdf, 0x55,0xaa,0x52,0x08};
char eics[9]        = {0xb0, 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char cas[9]         = {0xb1, 0x01,0x82,0x04,0x00,0x00,0x00,0x00,0x00};
char ifs[9]         = {0xb2, 0x00,0x00,0x00,0x00,0x01,0x00,0x16,0x00};
char dfs[5]         = {0xb3, 0x00,0x3a,0x00,0x0a};
char dfs_1[9]       = {0xb4, 0x02,0xcf,0x04,0xff,0x00,0x00,0x00,0xa4};
char dfs_2[9]       = {0xb5, 0x00,0x0d,0x00,0x00,0x00,0x04,0x00,0x00}; //PWM 6Khz
//char dfs_2[9]       = {0xb5, 0x00,0x06,0x00,0x00,0x00,0x04,0x00,0x00};  //PWM 13Khz
char pc_1[9]        = {0xb6, 0x49,0x07,0x10,0x00,0x00,0x53,0x44,0x00};
char dc[9]          = {0xb8, 0x73,0x0a,0x91,0x1e,0x00,0x08,0xb5,0xb5};
char vdcs[9]        = {0xb9, 0x00,0x01,0x37,0x00,0x00,0x00,0x00,0x00};
char gcev[13]       = {0xc0, 0x68,0xff,0x68,0xff,0x80,0xff,0x5b,0xff,0x5b,0xff,
                             0x71,0xff};
char gcpr[17]       = {0xc1, 0x71,0x7b,0x8b,0xa6,0xcd,0xc9,0x83,0xb0,0x64,0x82,
                             0x55,0x8e,0xa0,0xb4,0xc8,0xdc};
char gcpg[17]       = {0xc2, 0x71,0x7b,0x8b,0xa6,0xcd,0xc9,0x83,0xb0,0x64,0x82,
                             0x55,0x8e,0xa0,0xb4,0xc8,0xdc};
char gcpb[17]       = {0xc3, 0x85,0x8d,0x9b,0xb1,0xd4,0xce,0x8a,0xb5,0x68,0x85,
                             0x57,0x94,0xaa,0xbe,0xd2,0xe6};
char gcnr[17]       = {0xc4, 0x63,0x6d,0x7d,0x97,0xbd,0xb9,0x61,0x8d,0x3f,0x5d,
                             0x42,0x7a,0x96,0xaa,0xbe,0xd2};
char gcng[17]       = {0xc5, 0x63,0x6d,0x7d,0x97,0xbd,0xb9,0x61,0x8d,0x3f,0x5d,
                             0x42,0x7a,0x96,0xaa,0xbe,0xd2};
char gcnb[17]       = {0xc6, 0x77,0x7e,0x8c,0xa2,0xc4,0xbd,0x68,0x92,0x44,0x60,
                             0x44,0x80,0x96,0xaa,0xbe,0xd2};
//char pits[17]       = {0xc8, 0x11,0x18,0x0d,0x0d,0x28,0x12,0x00,0x00,0x00,0x00,
//                             0x00,0x00,0x00,0x00,0x00,0x00};
//char pits[17]       = {0xc8, 0x10,0x16,0x10,0x10,0x22,0x00,0x00,0x00,0x00,0x00,
//                             0x00,0x00,0x00,0x00,0x00,0x00};
char pits[17]       = {0xc8, 0x11,0x19,0x04,0x0d,0x28,0x08,0x00,0x04,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00};
char cscs_1[17]     = {0xca, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00};
char cscs_2[17]     = {0xcb, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00};
#endif
char sleep_out[2]   = {0x11, 0x00};
char disp_on[2]     = {0x29, 0x00};
char sleep_in[2]    = {0x10, 0x00};
char disp_off[2]    = {0x28, 0x00};

#ifdef FEATURE_ROHM_CABC_ON
char wdb[2]         = {0x51, 0x00};
char wctrld[2]      = {0x53, 0x24};
//char cabc_on[2]     = {0x55, 0x00}; //CABC OFF 
char cabc_on[2]     = {0x55, 0x03}; //MV MODE
//char cabc_on[2]     = {0x55, 0x02}; //ST MODE
//char cabc_on[2]     = {0x55, 0x01}; //UI MODE
//char cabc_min_val[2] = {0x5e, 0x00};   //100% limit
char cabc_min_val[2] = {0x5e, 0x66};   //60% limit 
//char cabc_min_val[2] = {0x5e, 0xb2};   //30% limit 
//char cabc_min_val[2] = {0x5e, 0xe5};   //10% limit 
#ifdef FEATURE_TP_SAMPLE
char cabc_dim_off[2]   ={0xf7, 0x25};
#endif
#endif

static struct dsi_cmd_desc rohm_display_init_cmds[] = {
#if defined (FEATURE_TP_SAMPLE)
    {DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_out), sleep_out},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_1), mtp_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_2), mtp_2} 
#elif defined (FEATURE_WS_SAMPLE)
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(eics), eics},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_1), mtp_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_2), mtp_2},   
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_3), mtp_3},    
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_4), mtp_4},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_5), mtp_5},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_6), mtp_6}, 
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(pw_lock), pw_lock}   
#else
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(eics), eics},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cas), cas},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(ifs), ifs},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(dfs), dfs},   
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(dfs_1), dfs_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(dfs_2), dfs_2},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(pc_1), pc_1},    
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(dc), dc},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(vdcs), vdcs},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev), gcev},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcpr), gcpr},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcpg), gcpg},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcpb), gcpb},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcnr), gcnr},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcng), gcng},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcnb), gcnb},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(pits), pits},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cscs_1), cscs_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cscs_2), cscs_2}
#endif
};


static struct dsi_cmd_desc rohm_display_on_cmds[] = {
#ifndef FEATURE_TP_SAMPLE	
    {DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_out), sleep_out},
#endif    
    {DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(disp_on), disp_on}
};

static struct dsi_cmd_desc rohm_display_off_cmds[] = {
    {DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(disp_off), disp_off},
    {DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(sleep_in), sleep_in}
};

#ifdef FEATURE_ROHM_CABC_ON
static struct dsi_cmd_desc rohm_display_cabc_on_cmds[] = {
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wdb), wdb},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wctrld), wctrld},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_on), cabc_on}
    ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_min_val), cabc_min_val}
#if defined (FEATURE_TP_SAMPLE)    
    ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl}   
    ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_dim_off), cabc_dim_off}
#endif
};

static struct dsi_cmd_desc rohm_display_cabc_bl_set_cmds[] = {
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wdb), wdb}
};     
#endif

#ifdef FEATURE_ROHM_ID_READ
static char manufacture_id[2] = {0xde, 0x00}; /* DTYPE_DCS_READ */
static char otp_status[2] = {0xd1, 0x00}; /* DTYPE_DCS_READ */

static struct dsi_cmd_desc rohm_manufacture_id_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(manufacture_id), manufacture_id};
static struct dsi_cmd_desc rohm_otp_status_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(otp_status), otp_status};


static uint32 mipi_rohm_manufacture_id(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	uint32 i;
	uint8 *lp;

       lp = NULL;
	tp = &rohm_tx_buf;
	rp = &rohm_rx_buf;
	cmd = &rohm_manufacture_id_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 4);
	for(i=0; i<4;i++)
	{
		lp = ((uint8 *)rp->data++);
		pr_info("%s: manufacture_id=0x%x", __func__, *lp);
		msleep(5);
	}
	return *lp;
}

static uint32 mipi_rohm_otp_status(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	uint32 i;
	uint8 *lp;

	tp = &rohm_tx_buf;
	rp = &rohm_rx_buf;
	cmd = &rohm_otp_status_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 8);
	for(i=0; i<4;i++)
	{
		lp = ((uint8 *)rp->data++);
		pr_info("%s: otp_status=0x%x", __func__, *lp );
		msleep(5);
	}
	return *lp;
}
#endif
#if 0
static char brightness_val[2] ={0x51, 0x00};
static struct dsi_cmd_desc rohm_brightness_cmd = {
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(brightness_val), brightness_val};

static uint32 mipi_rohm_brightness(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
       uint8 *lp;
	tp = &rohm_tx_buf;
	rp = &rohm_rx_buf;
	cmd = &rohm_brightness_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd, 4);
	lp = (uint8 *)rp->data;
	pr_info("%s: brightness_val=%x", __func__, *lp);
	return *lp;
}
#endif
/*
 0. rohm_display_init_cmds
 1. rohm_display_veil_init0_cmds
 2. rohm_display_veil_lut_cmds
 3. rohm_display_veil_init1_cmds
 4. rohm_display_veil_tex_cmds
 5. rohm_display_veil_colormap_cmds
 6. rohm_display_veil_init2_cmds
 7. dsi_cmd_desc rohm_display_on_cmds
 */

static int mipi_rohm_lcd_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

    ENTER_FUNC2();

	mfd = platform_get_drvdata(pdev);

	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;
#if defined (FEATURE_WS_SAMPLE)
	mipi_set_tx_power_mode(0);
#endif
	if (rohm_state.disp_initialized == false) {
		//PRINT("[LIVED] LCD RESET!!\n");
           	gpio_set_value_cansleep(gpio43, GPIO_LOW_VALUE); 
           	msleep(10);
		gpio_set_value_cansleep(gpio43, GPIO_HIGH_VALUE);
		msleep(10);
        
		mipi_dsi_cmds_tx(mfd, &rohm_tx_buf, rohm_display_init_cmds,
				ARRAY_SIZE(rohm_display_init_cmds));
		rohm_state.disp_initialized = true;
	}
	mipi_dsi_cmds_tx(mfd, &rohm_tx_buf, rohm_display_on_cmds,
			ARRAY_SIZE(rohm_display_on_cmds));
    rohm_state.disp_on = true;

#ifdef FEATURE_ROHM_CABC_ON
	mipi_dsi_cmds_tx(mfd, &rohm_tx_buf, rohm_display_cabc_on_cmds,
			ARRAY_SIZE(rohm_display_cabc_on_cmds));
#endif
#if defined (FEATURE_WS_SAMPLE)
	mipi_set_tx_power_mode(1);
#endif
#ifdef FEATURE_ROHM_ID_READ
mipi_set_tx_power_mode(0);
mipi_dsi_cmd_bta_sw_trigger(); 
mipi_rohm_manufacture_id(mfd);
mipi_rohm_otp_status(mfd);
mipi_set_tx_power_mode(1);
#endif
	EXIT_FUNC2();
	return 0;
}

static int mipi_rohm_lcd_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

    ENTER_FUNC2();

	mfd = platform_get_drvdata(pdev);

	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;

#if defined (FEATURE_WS_SAMPLE)
	mipi_set_tx_power_mode(0);
#endif

	if (rohm_state.disp_on == true) {
		//gpio_set_value_cansleep(gpio43, GPIO_LOW_VALUE);
		//usleep(10);
		//gpio_set_value_cansleep(gpio43, GPIO_HIGH_VALUE);
		//usleep(10);

		mipi_dsi_cmds_tx(mfd, &rohm_tx_buf, rohm_display_off_cmds,
				ARRAY_SIZE(rohm_display_off_cmds));
		rohm_state.disp_on = false;
		rohm_state.disp_initialized = false;
	}
#if defined (FEATURE_WS_SAMPLE)
	mipi_set_tx_power_mode(1);
#endif

       EXIT_FUNC2();
	return 0;
}


#if defined  FEATURE_SKY_BACKLIGHT_TPS61161 ||  defined FEATURE_ROHM_CABC_ON
static int first_enable = 0;
static int prev_bl_level = 0;
#endif

static void mipi_rohm_set_backlight(struct msm_fb_data_type *mfd)
{
#if defined (FEATURE_SKY_BACKLIGHT_TPS61161)
	int idx;
	unsigned long flags;
	int bl_level;

	bl_level = mfd->bl_level | 0x00;

	printk("mipi_rohm_set_backlight bl_level =%d, first_enable =%d, prev_bl_level =%d\n",bl_level, first_enable, prev_bl_level);

	if (prev_bl_level == 0) {
		mdelay(200);
	}

	if (bl_level == 0) {
		gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
        gpio_set_value_cansleep(gpio16, GPIO_LOW_VALUE);
		mdelay(T_SHUTDOWN); 
		first_enable =0;
	} else {
		if (first_enable == 0) {          
            gpio_set_value_cansleep(gpio16, GPIO_HIGH_VALUE);   
			local_save_flags(flags);
			local_irq_disable();
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			udelay(T_ES_DELAY); 
			gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
			udelay(T_ES_DETECT);
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			mdelay(T_ES_WIN); 

			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			udelay(T_START);
			for (idx=0; idx<NUM_ADDR_DIGIT; idx++) {
				uint8 bit = ((DEVICE_ADDR << idx) >>(NUM_ADDR_DIGIT-1)) & 0x01 ;

				if (bit == 1) {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_HB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_HB);  
				} else {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_LB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_LB);  

				}
			}
			gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
			udelay(T_EOS);

			// 5bit Data
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			udelay(T_START);

			for (idx=0; idx<NUM_DATA_DIGIT; idx++) {                          
				uint8 bit = (( bl_level<< idx) >>(NUM_DATA_DIGIT-1)) & 0x01 ;
				if (bit == 1) {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_HB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_HB);  
				} else {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_LB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_LB);  

				}
			}
			gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
			udelay(T_EOS);
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			
			local_irq_restore(flags);
			first_enable = 1;
		} else {

			//8bit device Address
			local_save_flags(flags);
			local_irq_disable();
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			udelay(T_START);
			for (idx=0; idx<NUM_ADDR_DIGIT; idx++) {
				uint8 bit = ((DEVICE_ADDR << idx) >>(NUM_ADDR_DIGIT-1)) & 0x01 ;

				if (bit == 1) {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_HB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_HB);  
				} else {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_LB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_LB);  

				}
			}
			gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
			udelay(T_EOS);

			// 5bit Data
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			udelay(T_START);

			for (idx=0; idx<NUM_DATA_DIGIT; idx++) {                          
				uint8 bit = (( bl_level<< idx) >>(NUM_DATA_DIGIT-1)) & 0x01 ;
				if (bit == 1) {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_HB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_HB);  
				} else {
					gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE); 
					udelay(T_LOW_LB);
					gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE); 
					udelay(T_HIGH_LB);  

				}
			}
			gpio_set_value_cansleep(gpio24, GPIO_LOW_VALUE);
			udelay(T_EOS);
			gpio_set_value_cansleep(gpio24, GPIO_HIGH_VALUE);
			local_irq_restore(flags);
		}            
	}

	prev_bl_level = bl_level;
#elif defined (FEATURE_ROHM_CABC_ON)
int bl_level;
    ENTER_FUNC2();

	if (prev_bl_level == mfd->bl_level)
		return;

    bl_level = mfd->bl_level *8;
    wdb[1] = bl_level;


	mutex_lock(&mfd->dma->ov_mutex);
    mipi_set_tx_power_mode(0);

    if(first_enable == 0)
    {
        gpio_set_value_cansleep(gpio16, GPIO_HIGH_VALUE);
        first_enable  = 1;
        
    }

	printk(KERN_ERR"mipi_rohm_set_backlight bl_level =%d \n",bl_level);

	mipi_dsi_cmds_tx(mfd, &rohm_tx_buf, rohm_display_cabc_bl_set_cmds,
			ARRAY_SIZE(rohm_display_cabc_bl_set_cmds));
    mipi_set_tx_power_mode(1);

	prev_bl_level = mfd->bl_level;
	mutex_unlock(&mfd->dma->ov_mutex);
    if(bl_level == 0)
    {
          gpio_set_value_cansleep(gpio16, GPIO_LOW_VALUE);
          first_enable = 0;
    }
    EXIT_FUNC2();
#endif /*FEATURE_SKY_BACKLIGHT_TPS61165*/
    
}


static int __devinit mipi_rohm_lcd_probe(struct platform_device *pdev)
{
    if (pdev->id == 0) {
        mipi_rohm_pdata = pdev->dev.platform_data;
		return 0;
	}

	msm_fb_add_device(pdev);

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_rohm_lcd_probe,
	.driver = {
		.name   = "mipi_ortus",
	},
};

static struct msm_fb_panel_data rohm_panel_data = {
       .on             = mipi_rohm_lcd_on,
       .off            = mipi_rohm_lcd_off,
       .set_backlight  = mipi_rohm_set_backlight,
};

static int ch_used[3];

int mipi_rohm_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel)
{
	struct platform_device *pdev = NULL;
	int ret;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_ortus", (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	rohm_panel_data.panel_info = *pinfo;

	ret = platform_device_add_data(pdev, &rohm_panel_data,
		sizeof(rohm_panel_data));
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_add_data failed!\n", __func__);
		goto err_device_put;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_register failed!\n", __func__);
		goto err_device_put;
	}

	return 0;

err_device_put:
	platform_device_put(pdev);
	return ret;
}

static int __init mipi_rohm_lcd_init(void)
{
    ENTER_FUNC2();

    rohm_state.disp_powered_up = true;

    mipi_dsi_buf_alloc(&rohm_tx_buf, DSI_BUF_SIZE);
    mipi_dsi_buf_alloc(&rohm_rx_buf, DSI_BUF_SIZE);

    EXIT_FUNC2();

    return platform_driver_register(&this_driver);
}

module_init(mipi_rohm_lcd_init);

