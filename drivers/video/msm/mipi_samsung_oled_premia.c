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
#include "mipi_samsung_oled_premia.h"
#include <mach/gpio.h>
#include <asm/irq.h>
#include <asm/system.h>

#include <linux/mfd/pm8xxx/pm8921.h>

#ifdef CONFIG_PANTECH_HDMI_PW_CTRL_ON_LCD
extern int hdmi_autodetect_control(int on);
extern int HDMI_Schedule_Control(int on);
extern uint32_t HDMI_Get_Cable_State(void);
#define HDMI_PW_ON 1
#define HDMI_PW_OFF 0
#endif

#define LCD_RESET		43

#define LCD_DEBUG_MSG

#ifdef LCD_DEBUG_MSG
#define ENTER_FUNC()        printk(KERN_INFO "[PANTECH_LCD] +%s \n", __FUNCTION__);
#define EXIT_FUNC()         printk(KERN_INFO "[PANTECH_LCD] -%s \n", __FUNCTION__);
#define ENTER_FUNC2()    printk(KERN_ERR "[PANTECH_LCD] +%s\n", __FUNCTION__);
#define EXIT_FUNC2()       printk(KERN_ERR "[PANTECH_LCD] -%s\n", __FUNCTION__);
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

#define D_SKY_BACKLIGHT_CONTROL		1
#if (BOARD_VER > WS10) 
#define OLED_DET	64
#endif

extern int gpio43;
static struct msm_panel_common_pdata *mipi_samsung_oled_hd_pdata;

static struct dsi_buf samsung_oled_hd_tx_buf;
static struct dsi_buf samsung_oled_hd_rx_buf;

struct lcd_state_type {
    boolean disp_powered_up;
    boolean disp_initialized;
    boolean disp_on;
};

static struct lcd_state_type samsung_oled_hd_state = { 0, };

unsigned char is_sleep,is_read=FALSE;
unsigned int prev_bl, now_bl;

uint8 elvss_id3;

char mtp_enable[3]		= {0xF1, 0x5A, 0x5A};
char sleep_out[2]       	= {0x11, 0x00};
char id_read[2]	        	= {0xD1, 0x00};
#if defined(CONFIG_MACH_MSM8960_VEGAPVW) && defined(MIPI_CLOCK_440MBPS)
char panel_cond_set[39]		= {0xF8, 0x19, 0x2E, 0x00, 0x00, 0x00, 0x83, 0x00, 0x35, 0x6E, 0x0E, 
	                              	 0x23, 0x07, 0x60, 0x00, 0x00, 0x00, 0x00, 0x04, 0x07, 0x60, 
                                	 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x1E, 0x60, 0xC0, 0xC1, 
                                 	 0x01, 0x81, 0xC1, 0x00, 0xC3, 0xF6, 0xF6, 0xC1};
#else
#if 1
#if defined(CONFIG_MACH_MSM8960_VEGAPVW) && (BOARD_VER >= WS10)
char panel_cond_set[39]		= {0xF8, 0x19, 0x35, 0x00, 0x00, 0x00, 0x95, 0x00, 0x3C, 0x7D, 0x10, 
	                              	 0x27, 0x08, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x6E, 
                                	 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x23, 0x6E, 0xC0, 0xC1, 
                                 	 0x01, 0x81, 0xC1, 0x00, 0xC3, 0xF6, 0xF6, 0xC1};
#else
char panel_cond_set[39]		= {0xF8, 0x3D, 0x35, 0x00, 0x00, 0x00, 0x93, 0x00, 0x3C, 0x7D, 0x08, 
                                	 0x27, 0x7D, 0x3F, 0x00, 0x00, 0x00, 0x20, 0x04, 0x08, 0x6E, 
                                	 0x00, 0x00, 0x00, 0x02, 0x08, 0x08, 0x23, 0x23, 0xC0, 0xC8, 
                                 	 0x08, 0x48, 0xC1, 0x00, 0xC1, 0xFF, 0xFF, 0xC8};
#endif//if (BOARD_VER >= WS10)

#else // using gpara 
char panel_cond_set[11]		= {0xF8, 0x3D, 0x35, 0x00, 0x00, 0x00, 0x93, 0x00, 0x3C, 0x7D, 0x08}; 
char panel_cond_set_gpara1[13]	= {0xB0, 0x0A, 0xF8, 0x27, 0x7D, 0x3F, 0x00, 0x00, 0x00, 0x20, 0x04, 0x08, 0x6E};
char panel_cond_set_gpara2[13]	= {0xB0, 0x14, 0xF8, 0x00, 0x00, 0x00, 0x02, 0x08, 0x08, 0x23, 0x23, 0xC0, 0xC8 };
char panel_cond_set_gpara3[11]	= {0xB0, 0x1E, 0xF8, 0x08, 0x48, 0xC1, 0x00, 0xC1, 0xFF, 0xFF, 0xC8};

#endif
#endif /* 440 */
char disp_cond_set[4]   	= {0xF2, 0x80, 0x03, 0x0D};
#if defined(CONFIG_MACH_MSM8960_VEGAPVW) && (BOARD_VER >= WS10)
char gamma_cond_set300[26] 	= {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA4, 0xB6, 0x95, 0xA9, 0xBC, 0xA2,
					 0xBB, 0xC9, 0xB6, 0x91, 0xA3, 0x8B, 0xAD, 0xB6, 0xA9, 0x00,
					 0xD6, 0x00, 0xBE, 0x00, 0xFC};
char gamma_cond_set20[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x64, 0x64, 0x63, 0xA1, 0xA5, 0x9E, 0xB2, 0xBE, 0xA8, 0x93, 0xA6, 0x8A, 0xBA, 0xC8, 0xB5, 0x00, 0x68, 0x00, 0x49, 0x00, 0x79};
#else
char gamma_cond_set[26] 	= {0xFA, 0x01, 0x58, 0x1F, 0x63, 0xAC, 0xB4, 0x99, 0xAD, 0xBA, 0xA3,
					 0xC0, 0xCB, 0xBB, 0x93, 0x9F, 0x8B, 0xAD, 0xB4, 0xA7, 0x00,
					 0xBE, 0x00, 0xAB, 0x00, 0xE7};
#endif//if defined(CONFIG_MACH_MSM8960_VEGAPVW) && (BOARD_VER >= WS10)
char gamma_update[2] 		= {0xF7, 0x03};
/* etc cond set */
char source_control[4]		= {0xF6, 0x00, 0x02, 0x00};
char pentile_control[10] 	= {0xB6, 0x0C, 0x02, 0x03, 0x32, 0xFF, 0x44, 0x44, 0xC0, 0x00};
char nvm_setting[15]		= {0xD9, 0x14, 0x40, 0x0C, 0xCB, 0xCE, 0x6E, 0xC4, 0x07, 0x40, 0x41,
							  		 0xD0, 0x00, 0x60, 0x19};
#if 0 /* not used */
char mipi_control1[6]		= {0xE1, 0x10, 0x1C, 0x17, 0x08, 0x1D};
char mipi_control2[7] 		= {0xE2, 0xED, 0x07, 0xC3, 0x13, 0x0D, 0x03};
char mipi_control3[2]		= {0xE3, 0x40};
char mipi_control4[8]		= {0xE4, 0x00, 0x00, 0x14, 0x80, 0x00, 0x00, 0x00};
#endif
#if defined(CONFIG_MACH_MSM8960_VEGAPVW) && (BOARD_VER >= WS10)
char power_control[8] 		= {0xF4, 0xCF, 0x0A, 0x15, 0x10, 0x1E, 0x33, 0x02};
#else
char power_control[8] 		= {0xF4, 0xCF, 0x0A, 0x0D, 0x10, 0x19, 0x33, 0x02};
#endif//if defined(CONFIG_MACH_MSM8960_VEGAPVW) && (BOARD_VER >= WS10)
char elvss_control[3] 		= {0xB1, 0x04, 0x95};
char disp_on[2]             = {0x29, 0x00};
char disp_off[2]            = {0x28, 0x00};
char sleep_in[2]            = {0x10, 0x00};

#ifdef D_SKY_BACKLIGHT_CONTROL
/* Samsung OLED Gamma Setting */
#if 0
char gamma_set_300[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA4, 0xB6, 0x95, 0xA9, 0xBC, 0xA2, 0xBB, 0xC9, 0xB6, 0x91, 0xA3, 0x8B, 0xAD, 0xB6, 0xA9, 0x00, 0xD6, 0x00, 0xBE, 0x00, 0xFC}; 
char gamma_set_280[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA6, 0x93, 0x97, 0xAA, 0xBD, 0xA3, 0xBB, 0xC9, 0xB6, 0x91, 0xA3, 0x8B, 0xAE, 0xB7, 0xA9, 0x00, 0xD1, 0x00, 0xB9, 0x00, 0xF7};
char gamma_set_260[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x9F, 0x8A, 0x91, 0xAC, 0xBE, 0xA5, 0xB9, 0xC8, 0xB4, 0x93, 0xA5, 0x8D, 0xAE, 0xB7, 0xA9, 0x00, 0xCD, 0x00, 0xB4, 0x00, 0xF2};
char gamma_set_240[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA1, 0x8D, 0x94, 0xAE, 0xBF, 0xA7, 0xB9, 0xC8, 0xB4, 0x93, 0xA6, 0x8D, 0xB0, 0xBA, 0xAB, 0x00, 0xC8, 0x00, 0xAE, 0x00, 0xEC};
char gamma_set_220[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x9D, 0x86, 0x91, 0xAD, 0xBF, 0xA5, 0xB9, 0xC8, 0xB4, 0x95, 0xA7, 0x8F, 0xB1, 0xBB, 0xAC, 0x00, 0xC3, 0x00, 0xA9, 0x00, 0xE6};
char gamma_set_200[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x95, 0x7E, 0x8B, 0xAF, 0xC0, 0xA7, 0xBB, 0xC9, 0xB6, 0x95, 0xA8, 0x8F, 0xB1, 0xBC, 0xAC, 0x00, 0xBE, 0x00, 0xA4, 0x00, 0xE1};
char gamma_set_180[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x98, 0x80, 0x8D, 0xB1, 0xC1, 0xA9, 0xBB, 0xCA, 0xB6, 0x97, 0xAA, 0x91, 0xB4, 0xBE, 0xAE, 0x00, 0xB8, 0x00, 0x9E, 0x00, 0xDA};
char gamma_set_160[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x8D, 0x74, 0x85, 0xB3, 0xC2, 0xAA, 0xBB, 0xC9, 0xB5, 0x99, 0xAC, 0x93, 0xB4, 0xBE, 0xAD, 0x00, 0xB3, 0x00, 0x98, 0x00, 0xD4};
char gamma_set_140[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x90, 0x76, 0x88, 0xB5, 0xC4, 0xAC, 0xBB, 0xC9, 0xB6, 0x9B, 0xAE, 0x95, 0xB7, 0xC2, 0xB0, 0x00, 0xAC, 0x00, 0x90, 0x00, 0xCD};
char gamma_set_120[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x86, 0x6B, 0x81, 0xB5, 0xC3, 0xAB, 0xBD, 0xCB, 0xB8, 0x9B, 0xAF, 0x95, 0xB9, 0xC4, 0xB1, 0x00, 0xA6, 0x00, 0x89, 0x00, 0xC5};
char gamma_set_100[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x7D, 0x5F, 0x79, 0xB5, 0xC3, 0xAA, 0xBE, 0xCC, 0xB9, 0x9D, 0xB1, 0x97, 0xBB, 0xC6, 0xB3, 0x00, 0x9F, 0x00, 0x82, 0x00, 0xBD};
char gamma_set_80[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x6C, 0x4D, 0x6B, 0xB8, 0xC5, 0xAC, 0xBF, 0xCD, 0xB9, 0x9E, 0xB2, 0x98, 0xBD, 0xC9, 0xB5, 0x00, 0x97, 0x00, 0x79, 0x00, 0xB4};
char gamma_set_60[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x5B, 0x39, 0x5C, 0xBA, 0xC7, 0xAF, 0xC3, 0xCF, 0xBC, 0xA0, 0xB4, 0x9B, 0xBF, 0xCB, 0xB7, 0x00, 0x8E, 0x00, 0x70, 0x00, 0xA9};
char gamma_set_40[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x4F, 0x2C, 0x52, 0xB3, 0xBF, 0xA9, 0xCA, 0xD3, 0xC2, 0xA4, 0xB8, 0x9E, 0xC2, 0xCE, 0xBA, 0x00, 0x83, 0x00, 0x64, 0x00, 0x9C};
//char gamma_set_30[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x3A, 0x17, 0x3D, 0xAA, 0xB2, 0xA3, 0xC9, 0xD2, 0xC0, 0xA8, 0xBA, 0xA0, 0xC4, 0xD1, 0xBD, 0x00, 0x7D, 0x00, 0x5D, 0x00, 0x94};
char gamma_set_20[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x27, 0x04, 0x2A, 0x97, 0x9B, 0x96, 0xC8, 0xD2, 0xBF, 0xAC, 0xBD, 0xA4, 0xC6, 0xD3, 0xBF, 0x00, 0x74, 0x00, 0x54, 0x00, 0x8A};
#else
char gamma_set_300[26] 	= {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA4, 0xB6, 0x95, 0xA9, 0xBC, 0xA2,
					 0xBB, 0xC9, 0xB6, 0x91, 0xA3, 0x8B, 0xAD, 0xB6, 0xA9, 0x00,
					 0xD6, 0x00, 0xBE, 0x00, 0xFC};
//char gamma_set_300[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA4, 0xB6, 0x95, 0xA9, 0xBC, 0xA2, 0xBB, 0xC9, 0xB6, 0x91, 0xA3, 0x8B, 0xAD, 0xB6, 0xA9, 0x00, 0xD6, 0x00, 0xBE, 0x00, 0xFC}; 
char gamma_set_280[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA7, 0xB9, 0x98, 0xAA, 0xBC, 0xA2, 0xBB, 0xC9, 0xB6, 0x90, 0xA2, 0x8A, 0xAD, 0xB7, 0xA9, 0x00, 0xD1, 0x00, 0xB8, 0x00, 0xF6};
char gamma_set_260[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA3, 0xB4, 0x95, 0xAB, 0xBC, 0xA3, 0xB9, 0xC7, 0xB4, 0x92, 0xA4, 0x8C, 0xAD, 0xB7, 0xA9, 0x00, 0xCC, 0x00, 0xB3, 0x00, 0xF0};
char gamma_set_240[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA7, 0xB7, 0x98, 0xAC, 0xBD, 0xA4, 0xB9, 0xC7, 0xB4, 0x92, 0xA4, 0x8B, 0xAF, 0xB9, 0xAB, 0x00, 0xC7, 0x00, 0xAD, 0x00, 0xEA};
char gamma_set_220[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA6, 0xB6, 0x99, 0xA8, 0xB9, 0xA0, 0xB9, 0xC8, 0xB4, 0x92, 0xA5, 0x8C, 0xB0, 0xBA, 0xAC, 0x00, 0xC1, 0x00, 0xA8, 0x00, 0xE3};
char gamma_set_200[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA2, 0xB1, 0x96, 0xA9, 0xB9, 0xA1, 0xBB, 0xC9, 0xB6, 0x92, 0xA5, 0x8B, 0xB0, 0xBA, 0xAC, 0x00, 0xBC, 0x00, 0xA2, 0x00, 0xDD};
char gamma_set_180[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA6, 0xB4, 0x9A, 0xAA, 0xBA, 0xA2, 0xBB, 0xCA, 0xB6, 0x92, 0xA5, 0x8C, 0xB2, 0xBC, 0xAE, 0x00, 0xB6, 0x00, 0x9C, 0x00, 0xD6};
char gamma_set_160[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA1, 0xAE, 0x96, 0xAA, 0xB8, 0xA1, 0xBA, 0xC9, 0xB5, 0x93, 0xA6, 0x8C, 0xB1, 0xBD, 0xAD, 0x00, 0xB0, 0x00, 0x95, 0x00, 0xCE};
char gamma_set_140[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA6, 0xB2, 0x9B, 0xAB, 0xB9, 0xA3, 0xBA, 0xC9, 0xB5, 0x94, 0xA8, 0x8E, 0xB4, 0xC0, 0xB0, 0x00, 0xA9, 0x00, 0x8D, 0x00, 0xC6};
char gamma_set_120[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA3, 0xAE, 0x9A, 0xA6, 0xB4, 0x9D, 0xBD, 0xCB, 0xB7, 0x92, 0xA7, 0x8B, 0xB6, 0xC2, 0xB2, 0x00, 0xA1, 0x00, 0x86, 0x00, 0xBD};
char gamma_set_100[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0xA1, 0xA9, 0x9A, 0xA2, 0xAE, 0x98, 0xBD, 0xCC, 0xB7, 0x93, 0xA8, 0x8C, 0xB7, 0xC3, 0xB3, 0x00, 0x9A, 0x00, 0x7E, 0x00, 0xB4};
char gamma_set_80[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x99, 0x9F, 0x94, 0xA2, 0xAF, 0x97, 0xBB, 0xC9, 0xB4, 0x92, 0xA7, 0x8B, 0xBA, 0xC6, 0xB5, 0x00, 0x91, 0x00, 0x74, 0x00, 0xA9};
char gamma_set_60[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x90, 0x93, 0x8D, 0xA3, 0xB1, 0x97, 0xBB, 0xC7, 0xB4, 0x93, 0xA8, 0x8B, 0xBA, 0xC7, 0xB5, 0x00, 0x87, 0x00, 0x6A, 0x00, 0x9D};
char gamma_set_40[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x8C, 0x8C, 0x8C, 0x9D, 0xAB, 0x91, 0xBC, 0xC5, 0xB5, 0x96, 0xAC, 0x8E, 0xB9, 0xC7, 0xB4, 0x00, 0x7A, 0x00, 0x5D, 0x00, 0x8F};
//char gamma_set_30[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x3A, 0x17, 0x3D, 0xAA, 0xB2, 0xA3, 0xC9, 0xD2, 0xC0, 0xA8, 0xBA, 0xA0, 0xC4, 0xD1, 0xBD, 0x00, 0x7D, 0x00, 0x5D, 0x00, 0x94};
char gamma_set_20[26] = {0xFA, 0x01, 0x71, 0x31, 0x7B, 0x64, 0x64, 0x63, 0xA1, 0xA5, 0x9E, 0xB2, 0xBE, 0xA8, 0x93, 0xA6, 0x8A, 0xBA, 0xC8, 0xB5, 0x00, 0x68, 0x00, 0x49, 0x00, 0x79};
#endif
#if defined(D_SKY_OLED_ACL)
char acl_per55[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x12,
			     0x1B, 0x23, 0x2C, 0x35, 0x3D, 0x46, 0x4E, 0x57};
char acl_per53[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0x11,
			     0x1A, 0x22, 0x2A, 0x32, 0x3A, 0x43, 0x4B, 0x53};
char acl_per52[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0x11,
			     0x19, 0x21, 0x29, 0x31, 0x39, 0x41, 0x49, 0x51};
char acl_per50[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0x10,
			     0x18, 0x1F, 0x27, 0x2E, 0x36, 0x3D, 0x45, 0x4C};
char acl_per48[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x0F,
			     0x17, 0x1E, 0x25, 0x2C, 0x33, 0x3B, 0x42, 0x49};
char acl_per45[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0E,
			     0x14, 0x1B, 0x21, 0x27, 0x2E, 0x34, 0x3B, 0x41};
char acl_per43[29] = { 0xC1, 0x47, 0x53, 0x13, 0x53, 0x00, 0x00, 0x02, 0xCF, 0x00, 0x00,
			     0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0D,
			     0x14, 0x1A, 0x20, 0x26, 0x2C, 0x33, 0x39, 0x3F};
char acl_on[2]	= { 0xC0, 0x01 };
char acl_off[2]	= { 0xC0, 0x00 };
#endif /* D_SKY_OLED_ACL */

#endif /* backlight */

static struct dsi_cmd_desc samsung_oled_hd_display_init_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_enable),mtp_enable }, 
	{DTYPE_DCS_WRITE, 1, 0, 0, 5, sizeof(sleep_out),sleep_out },  
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(panel_cond_set),panel_cond_set}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(disp_cond_set),disp_cond_set}, 
};

static struct dsi_cmd_desc samsung_oled_hd_screen_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_in), sleep_in},
};
/*
static struct dsi_cmd_desc samsung_oled_hd_display_sleepout_cmds[] = {

	{DTYPE_DCS_WRITE, 1, 0, 0, 5, sizeof(sleep_out),sleep_out },  
};
*/

static struct dsi_cmd_desc samsung_oled_hd_display_wakeup_cmds[] = {

	{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_out),sleep_out },  
#if defined(MIPI_CLOCK_440MBPS)
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mtp_enable),mtp_enable }, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(panel_cond_set),panel_cond_set}, 
#endif
};
/*
static struct dsi_cmd_desc samsung_oled_hd_display_on1_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(panel_cond_set),panel_cond_set}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(disp_cond_set),disp_cond_set}, 
};
*/
static struct dsi_cmd_desc samsung_oled_hd_display_on2_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(source_control),source_control}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(pentile_control),pentile_control}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(nvm_setting),nvm_setting}, 
#if 0
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mipi_control1),mipi_control1}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mipi_control2),mipi_control2}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mipi_control3),mipi_control3}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(mipi_control4),mipi_control4}, 
#endif
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(power_control),power_control}, 
	{DTYPE_DCS_LWRITE, 1, 0, 0, 120, sizeof(elvss_control),elvss_control}, 
	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(disp_on), disp_on},
};
#ifdef D_SKY_BACKLIGHT_CONTROL
/* backlight */
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_300[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_300), gamma_set_300},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_280[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_280), gamma_set_280},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_260[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_260), gamma_set_260},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_240[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_240), gamma_set_240},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_220[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_220), gamma_set_220},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_200[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_200), gamma_set_200},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_180[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_180), gamma_set_180},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_160[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_160), gamma_set_160},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_140[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_140), gamma_set_140},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_120[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_120), gamma_set_120},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_100[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_100), gamma_set_100},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_80[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_80), gamma_set_80},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_60[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_60), gamma_set_60},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_40[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_40), gamma_set_40},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
static struct dsi_cmd_desc samsung_oled_hd_display_gamma_cmds_20[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_set_20), gamma_set_20},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(gamma_update),gamma_update}, 
};
#endif

static struct dsi_cmd_desc samsung_oled_hd_id_cmd ={
	DTYPE_DCS_READ, 1, 0, 1, 5, sizeof(id_read), id_read};

static uint32 mipi_samsung_oled_hd_read_id(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	int i;
	uint8 *lp;

	tp = &samsung_oled_hd_tx_buf;
	rp = &samsung_oled_hd_rx_buf;	
		
	lp = NULL;

	cmd = &samsung_oled_hd_id_cmd;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd,4);
	for(i=0; i<3;i++)
	{
		lp = ((uint8 *)rp->data++);
		if(i==0)
		    printk("[PANTECH_LCD]OLED Manufacture ID(ID1) = 0x%x\n",*lp); 
		else if(i==1)
		    printk("[PANTECH_LCD]OLED Rev.(ID2) = 0x%x\n",*lp); 
		else if(i==2){
		    elvss_id3 = (*lp);
		    printk("[PANTECH_LCD]OLED ELVSS Value(ID3) = 0x%x\n",elvss_id3); 
	        }
		msleep(1);
	}
	
	return *lp;
}

#ifdef D_SKY_OLED_TEMP
void is_oled_temp_check(void)
{
	
	struct pm8xxx_adc_chan_result result;
	int rc=0;
	int try_max=0;

	do
	{
	   rc = pm8xxx_adc_mpp_config_read(PM8XXX_AMUX_MPP_11, ADC_MPP_1_AMUX6, &result);
	   try_max++;
	}while(rc && try_max<20);

   	printk("[PANTECH_LCD]%s: OLED Panel Temperature  %lld(uV)\n ", __func__, result.physical);
	
}
#endif

static int mipi_samsung_oled_hd_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
#if (BOARD_VER > WS10) 
	int noled_det;
#endif

    	ENTER_FUNC2();
    
	mfd = platform_get_drvdata(pdev);

	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;

       if (samsung_oled_hd_state.disp_initialized == false) {
    	    samsung_oled_hd_state.disp_initialized = true;
	   		
  	    	gpio_set_value_cansleep(gpio43, 0);
 	    	usleep(10);
            gpio_set_value_cansleep(gpio43, 1);  // lcd panel reset 
 	    	usleep(10);
         // mdelay(10);
           // mdelay(1);
		
            mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_init_cmds,
                   ARRAY_SIZE(samsung_oled_hd_display_init_cmds));

/*
            mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_sleepout_cmds,
                   ARRAY_SIZE(samsung_oled_hd_display_sleepout_cmds));

            mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_on1_cmds,
                   ARRAY_SIZE(samsung_oled_hd_display_on1_cmds));
*/			
#if (BOARD_VER > WS10) 
            noled_det = gpio_get_value(OLED_DET);
	    
	    if(noled_det)
		 printk("[PANTECH_LCD] OLED Panel Connector State : Disconnect !!!(%d)\n",noled_det);
	    else
		 printk("[PANTECH_LCD] OLED Panel Connector State : Connect !!!(%d)\n",noled_det);

	    
	    if((!is_read)&(!noled_det))
#else
 	    if(!is_read)
#endif
		{
			mipi_dsi_cmd_bta_sw_trigger(); 
	    	mipi_samsung_oled_hd_read_id(mfd);
	 	  	mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_300,
				ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_300));
			is_read = TRUE;
	    }else{
	  	 	mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_20,
				ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_20));
	    }
		
        
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_on2_cmds,
                  ARRAY_SIZE(samsung_oled_hd_display_on2_cmds));

	   is_sleep = FALSE;

	    printk("[PANTECH_LCD] power on state (oled_hd panel).... \n");
#ifdef D_SKY_OLED_TEMP
	 	    is_oled_temp_check();
#endif

  	   samsung_oled_hd_state.disp_on = true;
#ifdef CONFIG_PANTECH_HDMI_PW_CTRL_ON_LCD
		hdmi_autodetect_control(HDMI_PW_ON);
		HDMI_Schedule_Control(1);
#endif	
       }
       
       EXIT_FUNC2();
       return 0;
}

static int mipi_samsung_oled_hd_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
//	u32 tmp;
    
    	ENTER_FUNC2();
    
	mfd = platform_get_drvdata(pdev);

	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;
    
	if (samsung_oled_hd_state.disp_on == true) {
	//	is_sleep = TRUE;
        samsung_oled_hd_state.disp_initialized = false;
        samsung_oled_hd_state.disp_on = false;
        printk("[PANTECH_LCD] power off state (oled_hd panel).... \n");
#ifdef CONFIG_PANTECH_HDMI_PW_CTRL_ON_LCD
	if (!HDMI_Get_Cable_State()) {
		hdmi_autodetect_control(HDMI_PW_OFF);
		HDMI_Schedule_Control(0);
	}

#endif	
    	}	   
    
    	EXIT_FUNC2();
	return 0;
}


static void mipi_samsung_oled_hd_set_backlight(struct msm_fb_data_type *mfd)
{
	ENTER_FUNC2();

#ifdef D_SKY_BACKLIGHT_CONTROL
	if(prev_bl != mfd->bl_level) {
    		now_bl = mfd->bl_level; 
	} else {
		printk("[PANTECH_LCD:%s] Equal backlight value ... pass ....(prev_bl = %d, mfd->bl_level = %d)\n",__func__,prev_bl,mfd->bl_level);
		return;
	}

	printk(KERN_ERR "[PANTECH_LCD:] %s: Set : %d (is_stanby_mode:%d)\n", __FUNCTION__, (mfd->bl_level),is_sleep);

    if((samsung_oled_hd_state.disp_initialized != true) || (samsung_oled_hd_state.disp_on == false)) {
			printk("[PANTECH_LCD:%s] Panel is off state....Ignore backlight value... \n",__func__);
			return;
	}

	mutex_lock(&mfd->dma->ov_mutex);
    mipi_set_tx_power_mode(0);
	
	if((is_sleep==TRUE) && (samsung_oled_hd_state.disp_on == true)) {
	   is_sleep = FALSE;
           mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_wakeup_cmds,
                   ARRAY_SIZE(samsung_oled_hd_display_wakeup_cmds));
		   printk("[PANTECH_LCD:%s] wakeup state (oled_hd panel).... \n",__func__);
		   
	}

	switch (now_bl) {
	case 15:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_300,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_300));
		break;
	case 14:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_280,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_280));
		break;
	case 13:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_260,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_260));
		break;
	case 12:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_240,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_240));
		break;
	case 11:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_220,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_220));
		break;
	case 10:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_200,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_200));
		break;
	case 9:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_180,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_180));
		break;
	case 8:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_160,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_160));
		break;
	case 7:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_140,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_140));
		break;
	case 6:
		default:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_120,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_120));
		break;
	case 5:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_100,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_100));
		break;
	case 4:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_80,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_80));
		break;
	case 3:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_60,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_60));
		break;
	case 2:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_40,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_40));
		break;
	case 1:
	   mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_display_gamma_cmds_20,
			ARRAY_SIZE(samsung_oled_hd_display_gamma_cmds_20));
		break;
	case 0:
            mipi_dsi_cmds_tx(mfd, &samsung_oled_hd_tx_buf, samsung_oled_hd_screen_off_cmds,
             	      ARRAY_SIZE(samsung_oled_hd_screen_off_cmds));
	    is_sleep = TRUE;
	    printk("[PANTECH_LCD:%s] sleep state (oled_hd panel).... \n",__func__);
		break;
	}	

	prev_bl = now_bl;

   	mipi_set_tx_power_mode(1);
	mutex_unlock(&mfd->dma->ov_mutex);

#endif
	EXIT_FUNC2();
}

static struct msm_fb_panel_data samsung_oled_hd_panel_data = {
       .on             = mipi_samsung_oled_hd_on,
       .off            = mipi_samsung_oled_hd_off,
       .set_backlight  = mipi_samsung_oled_hd_set_backlight,
};

static int __devinit mipi_samsung_oled_hd_probe(struct platform_device *pdev)
{
    if (pdev->id == 0) {
        mipi_samsung_oled_hd_pdata = pdev->dev.platform_data;
		return 0;
	}

	msm_fb_add_device(pdev);

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_samsung_oled_hd_probe,
	.driver = {
		.name   = "mipi_samsung_oled_hd",
	},
};


static int ch_used[3];

int mipi_samsung_oled_hd_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel)
{
	struct platform_device *pdev = NULL;
	int ret;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_samsung_oled_hd", (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	samsung_oled_hd_panel_data.panel_info = *pinfo;

	ret = platform_device_add_data(pdev, &samsung_oled_hd_panel_data,
		sizeof(samsung_oled_hd_panel_data));
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

static int __init mipi_samsung_oled_hd_init(void)
{
    ENTER_FUNC2();

    samsung_oled_hd_state.disp_powered_up = true;

    mipi_dsi_buf_alloc(&samsung_oled_hd_tx_buf, DSI_BUF_SIZE);
    mipi_dsi_buf_alloc(&samsung_oled_hd_rx_buf, DSI_BUF_SIZE);

    EXIT_FUNC2();

    return platform_driver_register(&this_driver);
}
module_init(mipi_samsung_oled_hd_init);

