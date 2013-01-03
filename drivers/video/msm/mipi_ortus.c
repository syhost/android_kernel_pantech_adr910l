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
#include "mipi_ortus.h"
#include <mach/gpio.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/kernel.h>

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



extern int gpio43, gpio16, gpio24;; /* gpio43 :reset, gpio16:lcd bl */

static struct msm_panel_common_pdata *mipi_ortus_pdata;
static struct dsi_buf ortus_tx_buf;
static struct dsi_buf ortus_rx_buf;


#ifdef ORTUS_SYSFS_ACCESS
static struct kobject *panel_brightness_kobj = NULL;
static int panel_data,panel_ch;
enum{
	OFF =0,
	USER_IMAGE,
	PICTURE_IMG,
	MOVING_IMG

};

#endif
struct lcd_state_type {
    boolean disp_powered_up;
    boolean disp_initialized;
    boolean disp_on;
	boolean first_light;
};
static struct lcd_state_type ortus_state = { 0, };

#if(BOARD_VER >= TP15)

#if (BOARD_VER >= TP10)
#define FEATURE_TP_SAMPLE
#define FEATURE_ROHM_GAMMA_FIX
//#define FEATURE_CABC_BL_MIN_SET
#elif (BOARD_VER == WS20)
//#define FEATURE_WS_SAMPLE
#endif


static int first_enable = 0;
static int prev_bl_level = 0;

struct lcd_state_types {
    boolean disp_powered_up;
    boolean disp_initialized;
    boolean disp_on;
	struct mutex lcd_mutex;
#ifdef CONFIG_LCD_CABC_CONTROL
		int acl_flag;
#endif

};

static struct lcd_state_types rohm_state = { 0, };

#if defined (FEATURE_TP_SAMPLE)
#if defined (FEATURE_ROHM_GAMMA_FIX)
char extcctl[6]     = {0xdf, 0x55,0xaa,0x52,0x08,0x10};
char mtp_1[2]      = {0xf1, 0x08};
char mtp_2[4]      = {0xf7, 0x25,0x00,0x3d};
char dfs_2[9]       = {0xb5, 0x00,0x01,0x00,0x00,0x00,0x04,0x00,0x00}; //PWM 39.2Khz

//gamma 2.4
char gcev_1[13]       = {0xC0,0x22,0xEC,0x22,0xEC,0x3D,0xEC,0x03,0xEC,0x03,0xEC,0x1E,0xEC};
char gcev_2[17]       = {0xC1,0x2D,0x39,0x4E,0x6F,0x9E,0xA0,0x39,0x6E,0x2C,0x5D,0x49,0x75,0x83,0x8E,0x98,0xA2};
char gcev_3[17]       = {0xC2,0x2D,0x39,0x4E,0x6F,0x9E,0xA0,0x39,0x6E,0x2C,0x5D,0x49,0x75,0x83,0x8E,0x98,0xA2};
char gcev_4[17]       = {0xC3,0x45,0x4E,0x5F,0x7B,0xA5,0xA5,0x40,0x72,0x30,0x62,0x4C,0x69,0x7C,0x88,0x97,0xA8};
char gcev_5[17]       = {0xC4,0x0E,0x1A,0x2F,0x50,0x7F,0x82,0x00,0x32,0x00,0x23,0x2D,0x51,0x62,0x6E,0x78,0x82};
char gcev_6[17]       = {0xC5,0x0E,0x1A,0x2F,0x50,0x7F,0x82,0x00,0x32,0x00,0x23,0x2D,0x51,0x62,0x6E,0x78,0x82};
char gcev_7[17]       = {0xC6,0x26,0x2F,0x41,0x5D,0x87,0x87,0x05,0x37,0x00,0x28,0x30,0x49,0x58,0x64,0x6F,0x76};
#else
char extcctl[6]     = {0xdf, 0x55,0xaa,0x52,0x08,0x10};
char mtp_1[2]       = {0xf1, 0x08};
char mtp_2[4]      = {0xf7, 0x25,0x00,0x3d};
#endif
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
char cabc_off[2]     = {0x55, 0x00}; 

//char cabc_on[2]     = {0x55, 0x02}; //ST MODE
//char cabc_on[2]     = {0x55, 0x01}; //UI MODE
#ifdef FEATURE_CABC_BL_MIN_SET
char cabc_min_val[2] = {0x5e, 0x00};   //100% limit
//char cabc_min_val[2] = {0x5e, 0x66};   //60% limit 
//char cabc_min_val[2] = {0x5e, 0xb2};   //30% limit 
//char cabc_min_val[2] = {0x5e, 0xe5};   //10% limit 
#endif
#ifdef FEATURE_TP_SAMPLE
//char cabc_dim_off[2]   ={0xf7, 0x25};
char cabc_dim_off[2]   ={0xf7, 0x24};

#endif

#endif


static struct dsi_cmd_desc rohm_display_init_cmds[] = {
#if defined (FEATURE_TP_SAMPLE)
#if defined (FEATURE_ROHM_GAMMA_FIX)
    {DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_out), sleep_out},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_1), mtp_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_2), mtp_2}, 
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(dfs_2), dfs_2}, 
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_1), gcev_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_2), gcev_2},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_3), gcev_3}, 
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_4), gcev_4},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_5), gcev_5},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_6), gcev_6}, 
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gcev_7), gcev_7} 
#else
    {DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_out), sleep_out},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_1), mtp_1},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_2), mtp_2} 
#endif
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
    {DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_in), sleep_in}
};

#ifdef FEATURE_ROHM_CABC_ON
static struct dsi_cmd_desc rohm_display_cabc_on_cmds[] = {
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wdb), wdb},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wctrld), wctrld},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_on), cabc_on}
#ifdef  FEATURE_CABC_BL_MIN_SET
   ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_min_val), cabc_min_val}
#endif	
   ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(extcctl), extcctl}
   ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_dim_off), cabc_dim_off}	
};
#ifdef CONFIG_LCD_CABC_CONTROL
static struct dsi_cmd_desc rohm_display_cabc_off_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wdb), wdb},
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wctrld), wctrld},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_off), cabc_off}
};
#endif
static struct dsi_cmd_desc rohm_display_cabc_bl_set_cmds[] = {
    {DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(wdb), wdb}
#ifdef FEATURE_CABC_BL_MIN_SET 	
   ,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(cabc_min_val), cabc_min_val}
#endif    
};         
#endif


#endif



char SETEXTC[4]   = {0xB9,0xFF,0x83,0x94};

char SETPOWER[16] = {0xB1,0x7C,0x00,0x24,
					 0x06,0x01,0x10 ,0x10,
					 0x34,0x3C,0x2A,0x23,
					 0x57,0x12,0x01,0xE6};//0x34 ->0x24

char INVOFF[1]    = {0x20};

char MADCTL[2]	  = {0x36,0x00};

char COLMOD[2]	  = {0x3A,0x70};

char SETCYC[19]	  = {0xB4,0x00,0x00,0x00,
					 0x05,0x08,0x05,0x4C,
					 0x04,0x05,0x4C,0x23,
					 0x27,0x26,0xCA,0xCC,
					 0x02,0x05,0x04};	



char SETGIP[25]   = {0xD5,0x00,0x00,0x00,
					 0x01,0xCD,0x23,0xEf,
					 0x45,0x67,0x89,0xAB,
					 0xCC,0xCC,0xDC,0x10,
					 0xFE,0x32,0xBA,0x98,
					 0x76,0x54,0xCC,0xCC,
					 0xC0};


char SETGAMMA[35] = {0xE0,0x01,0x0A,0x13,
					0x0E,0x17,0x33,0x1C,
					0x28,0x44,0x4C,0x91,
					0x56,0x97,0x16,0x16,
					0x0E,0x11,0x01,0x0A,
					0x14,0x15,0x1F,0x3F,
					0x1E,0x2B,0x44,0x4D,
					0x91,0x55,0x98,0x16,
					0x16,0x10,0x11};

char SETCABC[10] = {0xc9,0x0F,0x00,0x1E,
					0x1E,0x00,0x00,0x00,
					0x01,0x3E};

char WRDISBV[2]  = {0x51,0x00};//0xE4
char WRCTRLD[2]  = {0x53,0x24};//0x24
char SETVDD[2]  = {0xBC,0x07};//0x24
char CABC_MIN[2] = {0x5E,0x66};  
char WRCABC[2]   = {0x55,0x03};


char SETPANEL[2]  = {0xCC,0x09};

char SETMIPI[2]   = {0xBA,0x03};
char SLPOUT[1]    = {0x11};
char DISPON[1]    = {0x29};
char DISPOFF[1]   = {0x28};
char SLPIN[1]     = {0x10};


static struct dsi_cmd_desc ortus_display_init_cmds[]=
{
	{DTYPE_DCS_WRITE,  1, 0, 0, 200, sizeof(SLPOUT), SLPOUT},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETEXTC), SETEXTC},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETPOWER), SETPOWER},
	{DTYPE_DCS_WRITE,  1, 0, 0, 0, sizeof(INVOFF), INVOFF},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(MADCTL), MADCTL},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(COLMOD), COLMOD},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETCYC), SETCYC},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETGIP), SETGIP},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETGAMMA), SETGAMMA},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(SETCABC), SETCABC},
	//{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(CABC_MIN), CABC_MIN},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(WRDISBV),WRDISBV },
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(WRCTRLD), WRCTRLD},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 5, sizeof(SETVDD), SETVDD},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 5, sizeof(WRCABC), WRCABC},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 50, sizeof(SETPANEL), SETPANEL},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(SETMIPI), SETMIPI}

};

static struct dsi_cmd_desc ortus_sleep_in_cmds[]=
{
	{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(DISPOFF), DISPOFF},
	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(SLPIN), SLPIN}
};
static struct dsi_cmd_desc ortus_sleep_out_cmds[]=
{
	//{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(SLPOUT), SLPOUT},
	{DTYPE_DCS_WRITE, 1, 0, 0, 50, sizeof(DISPON), DISPON}
};

static struct dsi_cmd_desc ortus_bl_cmds[]=
{
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(WRDISBV),WRDISBV }, //pwm
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(WRCTRLD), WRCTRLD}, //ctl
	{DTYPE_DCS_WRITE1, 1, 0, 0, 5, sizeof(WRCABC), WRCABC}   //user
};

#ifdef ORTUS_SYSFS_ACCESS
static ssize_t backlight_choice_show(struct  kobject *kobj, struct kobj_attribute  *attr, char *buf)
{
	return sprintf(buf, "%d\n", panel_ch);
}


static ssize_t backlight_choice_store(struct  kobject *kobj, struct kobj_attribute  *attr, const char *buf, size_t count)
{	
	
	sscanf(buf, "%du", &panel_ch);

	if(panel_ch == FALSE)
		WRCTRLD[1] |= (1<<2);
	else if(panel_ch == TRUE)
		WRCTRLD[1] &= (0<<2);

	return count;
}

static ssize_t panel_brightness_show(struct  kobject *kobj, struct kobj_attribute  *attr, char *buf)
{
	return sprintf(buf, "%d\n", panel_data);
}


static ssize_t panel_brightness_store(struct  kobject *kobj, struct kobj_attribute  *attr, const char *buf, size_t count)
{	
	
	sscanf(buf, "%du", &panel_data);

	if(panel_data == OFF){
		WRCABC[1] = OFF;
	}
	else if(panel_data == USER_IMAGE){
		WRCABC[1] = USER_IMAGE;
	}
	else if(panel_data ==PICTURE_IMG){
		WRCABC[1] = PICTURE_IMG;
	}
	else if(panel_data == MOVING_IMG){
		WRCABC[1] = MOVING_IMG;
	}
	printk(KERN_WARNING"[%s] WRCABC[1] = %d\n",__func__,WRCABC[1]);
	
	return count;
}


static struct kobj_attribute panel_brightness_attribute  = 
				__ATTR(panel_back, 0666, panel_brightness_show, panel_brightness_store);

static struct kobj_attribute back_choice_attribute  = 
				__ATTR(back_ch, 0666, backlight_choice_show, backlight_choice_store);

static struct attribute  *attrs[] = {
    &panel_brightness_attribute.attr,
	&back_choice_attribute.attr,
    NULL,   
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};
#endif
static int mipi_ortus_lcd_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

    //ENTER_FUNC2();
	mfd = platform_get_drvdata(pdev);
	
#if(BOARD_VER >= TP15)
	if(panel_choice == 1){
	
		if (!mfd)
			return -ENODEV;
		if (mfd->key != MFD_KEY)
			return -EINVAL;
		mutex_lock(&rohm_state.lcd_mutex);

			if (rohm_state.disp_initialized == false) {
				//PRINT("[LIVED] LCD RESET!!\n");
           		//gpio_set_value_cansleep(gpio43, GPIO_LOW_VALUE); 
           		//msleep(10);
				//gpio_set_value_cansleep(gpio43, GPIO_HIGH_VALUE);
				//msleep(10);
		
				mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_init_cmds,
					ARRAY_SIZE(rohm_display_init_cmds));
				rohm_state.disp_initialized = true;
			}
			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_on_cmds,
				ARRAY_SIZE(rohm_display_on_cmds));
      		rohm_state.disp_on = true;
#ifdef FEATURE_ROHM_CABC_ON
#ifdef CONFIG_LCD_CABC_CONTROL
			if(rohm_state.acl_flag == true){
				mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_off_cmds,
							ARRAY_SIZE(rohm_display_cabc_off_cmds));
				
			}
			else{ 

				mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_on_cmds,
						ARRAY_SIZE(rohm_display_cabc_on_cmds));
				
			}
#else
			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_on_cmds,
						ARRAY_SIZE(rohm_display_cabc_on_cmds));
#endif
#endif
			mutex_unlock(&rohm_state.lcd_mutex);


			printk(KERN_INFO "[SKY_LCD] -%s sharp  = %d\n", __FUNCTION__,rohm_state.acl_flag);
	}
	else
#endif
	{
		if (!mfd)
			return -ENODEV;
		if (mfd->key != MFD_KEY)
			return -EINVAL;

		if (ortus_state.disp_initialized == false) {
			
			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_display_init_cmds,
					ARRAY_SIZE(ortus_display_init_cmds));
			ortus_state.disp_initialized = true;
		}
		mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_sleep_out_cmds,
				ARRAY_SIZE(ortus_sleep_out_cmds));
		ortus_state.disp_on = true;
		ortus_state.first_light =true;
		printk(KERN_INFO "[SKY_LCD] -%s ortus\n", __FUNCTION__);
	}
	
	
	return 0;
}

static int mipi_ortus_lcd_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

   // ENTER_FUNC2();

	mfd = platform_get_drvdata(pdev);
#if(BOARD_VER >= TP15)
	if(panel_choice == 1){

		if (!mfd)
			return -ENODEV;
		if (mfd->key != MFD_KEY)
			return -EINVAL;


	if (rohm_state.disp_on == true) {
		//gpio_set_value_cansleep(gpio43, GPIO_LOW_VALUE);
		//msleep(5);
		//gpio_set_value_cansleep(gpio43, GPIO_HIGH_VALUE);
		//msleep(5);
		mutex_lock(&rohm_state.lcd_mutex);
		mipi_set_tx_power_mode(0);
		mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_off_cmds,
				ARRAY_SIZE(rohm_display_off_cmds));
		
		rohm_state.disp_on = false;
		rohm_state.disp_initialized = false;
		mipi_set_tx_power_mode(1);
		mutex_unlock(&rohm_state.lcd_mutex);

	}

	}else
#endif
	{
		if (!mfd)
			return -ENODEV;
		if (mfd->key != MFD_KEY)
			return -EINVAL;

		if (ortus_state.disp_on == true) {
		

			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_sleep_in_cmds,
					ARRAY_SIZE(ortus_sleep_in_cmds));
			ortus_state.disp_on = false;
			ortus_state.disp_initialized = false;
		}
	   
		ortus_state.first_light =false;
	} 
	EXIT_FUNC2();
	return 0;
}
#ifdef CONFIG_LCD_CABC_CONTROL
void cabc_contol(struct msm_fb_data_type *mfd, int state)
{
	mutex_lock(&rohm_state.lcd_mutex);
	mipi_set_tx_power_mode(0);
	if(state == true){
		mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_off_cmds,
								ARRAY_SIZE(rohm_display_cabc_off_cmds));

		rohm_state.acl_flag = true;
		printk(KERN_WARNING"mipi_sharp CABC = OFF\n");
	}
	else{ 
		mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_on_cmds,
							ARRAY_SIZE(rohm_display_cabc_on_cmds));
		rohm_state.acl_flag = false;
		printk(KERN_WARNING"mipi_sharp CABC = ON\n");
	}
	
	mipi_set_tx_power_mode(1);	
	mutex_unlock(&rohm_state.lcd_mutex);



}
#endif
static void mipi_ortus_set_backlight(struct msm_fb_data_type *mfd)
{



#if(BOARD_VER >= TP15)

	int bl_level;
	if(panel_choice == 1){
	    //ENTER_FUNC2();

		if (prev_bl_level == mfd->bl_level)
			return;

	     bl_level = mfd->bl_level;
	
		//printk(KERN_ERR"mipi_rohm_set_backlight bl_level =%d \n",bl_level);
		switch(bl_level){
			case 16:
				wdb[1] = 255;
				break;
			case 15:
				wdb[1] = 242;
				break;
			case 14:
				wdb[1] = 230;
				break;
			case 13:
				wdb[1] = 216;
				break;
			case 12:
				wdb[1] = 204;
				break;
			case 11:
				wdb[1] = 191;
				break;
			case 10:
				wdb[1] = 178;
				break;
			case 9:
				wdb[1] = 165;
				break;
			case 8:
				wdb[1] = 152;
				break;
			case 7:
				wdb[1] = 140;
				break;
			case 6:
				wdb[1] = 125;
				break;
			case 5:
				wdb[1] = 113;
				break;
			case 4:
				wdb[1] = 100;
				break;
			case 3:
				wdb[1] = 88;
				break;
			case 2:
				wdb[1] = 76;
				break;
			case 1:
				wdb[1] = 63;
				break;
			case 0:
				wdb[1] = 0;
				break;	
		}	
		if(first_enable == 0)
		{
			gpio_set_value_cansleep(gpio16, GPIO_HIGH_VALUE);
			first_enable  = 1;
				
		}
			
		
		mutex_lock(&rohm_state.lcd_mutex);
		mipi_set_tx_power_mode(0);
		if(charger_flag == 1 && bl_level > 0){
			
			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_on_cmds,
					ARRAY_SIZE(rohm_display_on_cmds));
		}
		
		mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_cabc_bl_set_cmds,
					ARRAY_SIZE(rohm_display_cabc_bl_set_cmds));
		
		if(charger_flag == 1 && bl_level == 0){
			mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, rohm_display_off_cmds,
					ARRAY_SIZE(rohm_display_off_cmds));
		}
		
			
		prev_bl_level = mfd->bl_level;
		mipi_set_tx_power_mode(1);
		mutex_unlock(&rohm_state.lcd_mutex);	
		if(bl_level == 0)
		{
			gpio_set_value_cansleep(gpio16, GPIO_LOW_VALUE);
			 first_enable = 0;
		}
		printk(KERN_ERR"mipi_rohm_set_backlight bl_level =%d \n",bl_level);
	  
	}
	else
#endif
	{
		if(!mfd)
			return;
		if(mfd->key != MFD_KEY)
			return;


		if(ortus_state.first_light){
			gpio_set_value_cansleep(gpio16, GPIO_HIGH_VALUE);
			ortus_state.first_light =false;
		}
		mutex_lock(&mfd->dma->ov_mutex);
		mipi_set_tx_power_mode(0);

		
		switch(mfd->bl_level){
			case 16 : WRDISBV[1] = 255;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 15 : WRDISBV[1] = 240;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 14 : WRDISBV[1] = 224;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 13 : WRDISBV[1] = 208;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 12 : WRDISBV[1] = 192;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 11 : WRDISBV[1] = 176;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 10 : WRDISBV[1] = 160;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 9 : WRDISBV[1] = 144;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 8 : WRDISBV[1] = 128;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 7 : WRDISBV[1] = 112;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 6 : WRDISBV[1] = 96;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 5 : WRDISBV[1] = 80;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 4 : WRDISBV[1] = 64;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 3 : WRDISBV[1] = 48;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 2 : WRDISBV[1] = 32;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			case 1 : WRDISBV[1] = 16;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			default : WRDISBV[1] = 0;
					  mipi_dsi_cmds_tx(mfd, &ortus_tx_buf, ortus_bl_cmds,
							ARRAY_SIZE(ortus_bl_cmds));
					  break;
			}
		

		mipi_set_tx_power_mode(1);
		mutex_unlock(&mfd->dma->ov_mutex);
		printk(KERN_WARNING"[%s] = %d\n",__func__,WRDISBV[1]);
	}

}


static int __devinit mipi_ortus_lcd_probe(struct platform_device *pdev)
{
#ifdef ORTUS_SYSFS_ACCESS
	struct device *dev = &pdev->dev;
	int retval;

	
	panel_brightness_kobj = kobject_create_and_add("panel_backlight",&dev->kobj);
	printk(KERN_DEBUG "%s : kobject create failed \n",__func__);
	if (!panel_brightness_kobj){
        return -ENOMEM;
	}

	retval = sysfs_create_group(panel_brightness_kobj,  &attr_group);

	if(retval){
		printk(KERN_DEBUG "%s : sysfs create file failed\n",__func__);
		kobject_put(panel_brightness_kobj);
	}
#endif	
	if (pdev->id == 0) {
        mipi_ortus_pdata = pdev->dev.platform_data;
		return 0;
	}
	mutex_init(&rohm_state.lcd_mutex);
	msm_fb_add_device(pdev);

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_ortus_lcd_probe,
	.driver = {
		.name   = "mipi_ortus",
	},
};

static struct msm_fb_panel_data ortus_panel_data = {
       .on             = mipi_ortus_lcd_on,
       .off            = mipi_ortus_lcd_off,
       .set_backlight  = mipi_ortus_set_backlight,
};

static int ch_used[3];

int mipi_ortus_device_register(struct msm_panel_info *pinfo,
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

	ortus_panel_data.panel_info = *pinfo;

	ret = platform_device_add_data(pdev, &ortus_panel_data,
		sizeof(ortus_panel_data));
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

static int __init mipi_ortus_lcd_init(void)
{
    ENTER_FUNC2();

    ortus_state.disp_powered_up = true;

    mipi_dsi_buf_alloc(&ortus_tx_buf, DSI_BUF_SIZE);
    mipi_dsi_buf_alloc(&ortus_rx_buf, DSI_BUF_SIZE);

    EXIT_FUNC2();

    return platform_driver_register(&this_driver);
}

module_init(mipi_ortus_lcd_init);

