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
#include "mipi_samsung_octa.h"
#include <mach/gpio.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "mdp4.h"

#define GPIO_HIGH_VALUE 1
#define GPIO_LOW_VALUE  0

#define NOP()	do {asm volatile ("NOP");} while(0);
#define DELAY_3NS() do { \
    asm volatile ("NOP"); \
    asm volatile ("NOP"); \
    asm volatile ("NOP");} while(0);

#define LCD_DEBUG_MSG
#define SAMSUNG_LCD_MIPI_CELL
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

extern int gpio43;
#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
void mtp_read(void);
void mtp_work_fnc(struct work_struct *work);
int chnnel_mtp =false;
#endif
struct lcd_state_type {
    boolean disp_powered_up;
    boolean disp_initialized;
    boolean disp_on;
	boolean backlightoff;
#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
	struct delayed_work mtp_work;
#endif
#ifdef SAMSUNG_LCD_MIPI_CELL
	boolean id_read_flag;
	int id_read_val;
#endif
#ifdef CONFIG_ACL_FOR_AMOLED
	boolean acl_flag;
	int acl_data;

#endif
};

int backlight_cnt;
int backlight_control;

int first;
int chargerFlag = 0;
static struct lcd_state_type oscar_state = { 0, };


static struct msm_panel_common_pdata *mipi_oscar_pdata;
static struct dsi_buf oscar_tx_buf;
static struct dsi_buf oscar_rx_buf;


#if(BOARD_VER <= WS20)
char Gamma_Set300[26] = {0xfa,0x02,0x58,0x42,0x66,0xAA,0xF1,0xAE,0xB5,0xC1,0xBE,0xB4,0xC0,0xB2,0x93,0x9F,0x93,0xA6,0xAD,0xA2,0x00,0xE9,0x00,0xDB,0x01,0x0F};
char Gamma_Set290[26] = {0xfa,0x02,0x58,0x42,0x66,0xAD,0xF3,0xB1,0xB3,0xC1,0xBB,0xB3,0xBF,0xB2,0x93,0x9F,0x93,0xA6,0xAD,0xA2,0x00,0xE5,0x00,0xD7,0x01,0x0B};
char Gamma_Set280[26] = {0xfa,0x02,0x58,0x42,0x66,0xAD,0xF3,0xB1,0xB5,0xC3,0xBE,0xB5,0xC1,0xB4,0x91,0x9E,0x91,0xA7,0xAE,0xA3,0x00,0xE2,0x00,0xD4,0x01,0x07};
char Gamma_Set270[26] = {0xfa,0x02,0x58,0x42,0x66,0xA9,0xEA,0xAD,0xB3,0xC3,0xBB,0xB5,0xC1,0xB4,0x92,0x9F,0x92,0xA7,0xAE,0xA4,0x00,0xDF,0x00,0xD1,0x01,0x04};
char Gamma_Set260[26] = {0xfa,0x02,0x58,0x42,0x66,0xA9,0xEA,0xAD,0xB5,0xC5,0xBE,0xB4,0xC0,0xB4,0x93,0x9F,0x93,0xA8,0xAF,0xA4,0x00,0xDC,0x00,0xCE,0x01,0x00};
char Gamma_Set250[26] = {0xfa,0x02,0x58,0x42,0x66,0xAC,0xED,0xB0,0xB3,0xC5,0xBB,0xB6,0xC2,0xB6,0x92,0x9F,0x92,0xA8,0xAF,0xA5,0x00,0xD9,0x00,0xCB,0x00,0xFC};
char Gamma_Set240[26] = {0xfa,0x02,0x58,0x42,0x66,0xAC,0xED,0xB0,0xB6,0xC7,0xBE,0xB6,0xC2,0xB6,0x92,0x9F,0x91,0xAA,0xB1,0xA7,0x00,0xD5,0x00,0xC8,0x00,0xF8};
char Gamma_Set230[26] = {0xfa,0x02,0x58,0x42,0x66,0xA8,0xE4,0xAC,0xB4,0xC7,0xBB,0xB8,0xC3,0xB9,0x91,0x9E,0x90,0xAB,0xB2,0xA8,0x00,0xD1,0x00,0xC5,0x00,0xF4};
char Gamma_Set220[26] = {0xfa,0x02,0x58,0x42,0x66,0xAC,0xE6,0xAF,0xB2,0xC7,0xB9,0xB8,0xC3,0xB9,0x92,0x9F,0x91,0xAC,0xB3,0xA9,0x00,0xCD,0x00,0xC1,0x00,0xEF};
char Gamma_Set210[26] = {0xfa,0x02,0x58,0x42,0x66,0xAC,0xE6,0xAF,0xB4,0xCA,0xBB,0xBA,0xC5,0xBB,0x91,0x9F,0x91,0xAB,0xB3,0xA9,0x00,0xCA,0x00,0xBE,0x00,0xEC};
char Gamma_Set200[26] = {0xfa,0x02,0x58,0x42,0x66,0xA7,0xDE,0xAA,0xB2,0xCA,0xB9,0xBC,0xC7,0xBE,0x90,0x9E,0x90,0xAC,0xB4,0xAA,0x00,0xC6,0x00,0xBA,0x00,0xE7};
char Gamma_Set190[26] = {0xfa,0x02,0x58,0x42,0x66,0xAB,0xE0,0xAE,0xB0,0xCA,0xB6,0xBB,0xC6,0xBE,0x91,0x9F,0x90,0xAE,0xB5,0xAC,0x00,0xC2,0x00,0xB6,0x00,0xE3};
char Gamma_Set180[26] = {0xfa,0x02,0x58,0x42,0x66,0xAB,0xE0,0xAE,0xB3,0xCC,0xB9,0xBE,0xC8,0xC1,0x90,0x9E,0x8F,0xAF,0xB7,0xAD,0x00,0xBE,0x00,0xB2,0x00,0xDE};
char Gamma_Set170[26] = {0xfa,0x02,0x58,0x42,0x66,0xAF,0xE2,0xB2,0xB1,0xCD,0xB6,0xC0,0xCA,0xC4,0x90,0x9E,0x8F,0xB0,0xB7,0xAE,0x00,0xBA,0x00,0xAF,0x00,0xDA};
char Gamma_Set160[26] = {0xfa,0x02,0x58,0x42,0x66,0xA6,0xD4,0xA9,0xB1,0xCF,0xB6,0xBF,0xCA,0xC4,0x90,0x9E,0x8E,0xB0,0xB8,0xAF,0x00,0xB6,0x00,0xAB,0x00,0xD5};
char Gamma_Set150[26] = {0xfa,0x02,0x58,0x42,0x66,0xA6,0xD4,0xA9,0xB4,0xD2,0xB9,0xC2,0xCC,0xC7,0x8E,0x9D,0x8D,0xB3,0xBA,0xB2,0x00,0xB2,0x00,0xA6,0x00,0xCF};
char Gamma_Set140[26] = {0xfa,0x02,0x58,0x42,0x66,0xAA,0xD6,0xAC,0xB1,0xD2,0xB6,0xC2,0xCC,0xC7,0x90,0x9F,0x8F,0xB4,0xBC,0xB3,0x00,0xAD,0x00,0xA2,0x00,0xCA};
char Gamma_Set130[26] = {0xfa,0x02,0x58,0x42,0x66,0xA2,0xC8,0xA4,0xB0,0xD4,0xB4,0xC4,0xCD,0xCB,0x8E,0x9D,0x8C,0xB6,0xBE,0xB5,0x00,0xA8,0x00,0x9D,0x00,0xC5};
char Gamma_Set120[26] = {0xfa,0x02,0x58,0x42,0x66,0xA7,0xCB,0xA9,0xAC,0xD3,0xB0,0xC6,0xCF,0xCD,0x8E,0x9E,0x8D,0xB7,0xBF,0xB7,0x00,0xA4,0x00,0x99,0x00,0xC0};
char Gamma_Set110[26] = {0xfa,0x02,0x58,0x42,0x66,0x9E,0xBC,0xA0,0xAB,0xD6,0xAE,0xC5,0xCE,0xCC,0x8F,0x9F,0x8F,0xB6,0xBF,0xB6,0x00,0x9F,0x00,0x95,0x00,0xBB};
char Gamma_Set100[26] = {0xfa,0x02,0x58,0x42,0x66,0xA4,0xBF,0xA6,0xA6,0xD5,0xA9,0xC7,0xD0,0xCE,0x90,0xA0,0x90,0xB8,0xC1,0xB8,0x00,0x9B,0x00,0x90,0x00,0xB6};
char Gamma_Set90[26] = {0xfa,0x02,0x58,0x42,0x66,0xAA,0xC1,0xAC,0xA6,0xD7,0xA8,0xC7,0xD3,0xCE,0x8F,0x9F,0x90,0xB9,0xC1,0xB9,0x00,0x96,0x00,0x8C,0x00,0xB0};
char Gamma_Set80[26] = {0xfa,0x02,0x58,0x42,0x66,0x9B,0xAD,0x9C,0xA6,0xD8,0xA9,0xC3,0xD3,0xC9,0x91,0xA1,0x92,0xBB,0xC3,0xBB,0x00,0x90,0x00,0x87,0x00,0xAA};
char Gamma_Set70[26] = {0xfa,0x02,0x58,0x42,0x66,0xA1,0xAF,0xA2,0xA7,0xD9,0xAA,0xC2,0xD6,0xC7,0x91,0xA1,0x93,0xBB,0xC4,0xBB,0x00,0x8B,0x00,0x81,0x00,0xA4};
#else
#if 0
char Gamma_Set300[26] = {0xfa,0x02,0x58,0x42,0x56,0xA2,0xC8,0xAE,0xB5,0xC1,0xBE,0xB4,0xC0,0xB2,0x93,0x9F,0x93,0xA6,0xAD,0xA2,0x00,0xE9,0x00,0xDB,0x01,0x0F};
char Gamma_Set290[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xCA,0xB1,0xB2,0xBF,0xBB,0xB3,0xBF,0xB2,0x93,0x9F,0x93,0xA6,0xAD,0xA2,0x00,0xE5,0x00,0xD8,0x01,0x0B};
char Gamma_Set280[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xCA,0xB1,0xB5,0xC1,0xBE,0xB5,0xC1,0xB4,0x91,0x9D,0x91,0xA7,0xAE,0xA3,0x00,0xE2,0x00,0xD5,0x01,0x07};
char Gamma_Set270[26] = {0xfa,0x02,0x58,0x42,0x56,0xA1,0xC5,0xAD,0xB2,0xBF,0xBB,0xB5,0xC1,0xB4,0x92,0x9E,0x92,0xA7,0xAE,0xA4,0x00,0xDF,0x00,0xD2,0x01,0x03};
char Gamma_Set260[26] = {0xfa,0x02,0x58,0x42,0x56,0xA1,0xC5,0xAD,0xB5,0xC1,0xBE,0xB4,0xC1,0xB4,0x93,0x9F,0x93,0xA8,0xAF,0xA4,0x00,0xDC,0x00,0xCF,0x00,0xFF};
char Gamma_Set250[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xC8,0xB0,0xB3,0xBF,0xBB,0xB6,0xC3,0xB6,0x92,0x9E,0x92,0xA8,0xAF,0xA5,0x00,0xD9,0x00,0xCC,0x00,0xFB};
char Gamma_Set240[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xC8,0xB0,0xB5,0xC1,0xBE,0xB6,0xC3,0xB6,0x92,0x9E,0x91,0xAA,0xB1,0xA7,0x00,0xD5,0x00,0xC9,0x00,0xF6};
char Gamma_Set230[26] = {0xfa,0x02,0x58,0x42,0x56,0xA1,0xC2,0xAC,0xB3,0xC0,0xBB,0xB8,0xC5,0xB9,0x91,0x9D,0x90,0xAB,0xB2,0xA8,0x00,0xD1,0x00,0xC6,0x00,0xF2};
char Gamma_Set220[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xC5,0xAF,0xB0,0xBE,0xB9,0xB8,0xC5,0xB9,0x92,0x9D,0x91,0xAC,0xB3,0xA9,0x00,0xCD,0x00,0xC2,0x00,0xEE};
char Gamma_Set210[26] = {0xfa,0x02,0x58,0x42,0x56,0xA5,0xC5,0xAF,0xB3,0xC0,0xBB,0xBA,0xC7,0xBB,0x91,0x9D,0x91,0xAB,0xB3,0xA9,0x00,0xCA,0x00,0xBF,0x00,0xEA};
char Gamma_Set200[26] = {0xfa,0x02,0x58,0x42,0x56,0xA1,0xBF,0xAA,0xB1,0xBE,0xB9,0xBC,0xC9,0xBE,0x90,0x9C,0x90,0xAC,0xB4,0xAA,0x00,0xC6,0x00,0xBC,0x00,0xE5};
char Gamma_Set190[26] = {0xfa,0x02,0x58,0x42,0x56,0xA4,0xC2,0xAE,0xAE,0xBC,0xB6,0xBB,0xC9,0xBE,0x91,0x9D,0x90,0xAE,0xB5,0xAC,0x00,0xC2,0x00,0xB8,0x00,0xE0};
char Gamma_Set180[26] = {0xfa,0x02,0x58,0x42,0x56,0xA4,0xC2,0xAE,0xB1,0xBE,0xB9,0xBE,0xCB,0xC1,0x90,0x9C,0x8F,0xAF,0xB7,0xAD,0x00,0xBE,0x00,0xB4,0x00,0xDB};
char Gamma_Set170[26] = {0xfa,0x02,0x58,0x42,0x56,0xA8,0xC5,0xB2,0xAE,0xBD,0xB6,0xC0,0xCE,0xC4,0x90,0x9C,0x8F,0xB0,0xB7,0xAE,0x00,0xBA,0x00,0xB1,0x00,0xD6};
char Gamma_Set160[26] = {0xfa,0x02,0x58,0x42,0x56,0xA0,0xBA,0xA9,0xAE,0xBD,0xB6,0xBF,0xCD,0xC4,0x90,0x9C,0x8E,0xB0,0xB8,0xAF,0x00,0xB6,0x00,0xAD,0x00,0xD1};
char Gamma_Set150[26] = {0xfa,0x02,0x58,0x42,0x56,0xA0,0xBA,0xA9,0xB1,0xBF,0xB9,0xC2,0xD0,0xC7,0x8E,0x9B,0x8D,0xB3,0xBA,0xB2,0x00,0xB2,0x00,0xA9,0x00,0xCC};
char Gamma_Set140[26] = {0xfa,0x02,0x58,0x42,0x56,0xA4,0xBE,0xAC,0xAE,0xBD,0xB6,0xC2,0xD0,0xC7,0x90,0x9C,0x8F,0xB4,0xBC,0xB3,0x00,0xAD,0x00,0xA4,0x00,0xC6};
char Gamma_Set130[26] = {0xfa,0x02,0x58,0x42,0x56,0x9D,0xB3,0xA4,0xAD,0xBD,0xB4,0xC4,0xD2,0xCB,0x8E,0x9A,0x8C,0xB6,0xBE,0xB5,0x00,0xA8,0x00,0xA0,0x00,0xC0};
char Gamma_Set120[26] = {0xfa,0x02,0x58,0x42,0x56,0xA2,0xB7,0xA9,0xA8,0xB9,0xB0,0xC6,0xD4,0xCD,0x8E,0x9A,0x8D,0xB7,0xBF,0xB7,0x00,0xA4,0x00,0x9C,0x00,0xBB};
char Gamma_Set110[26] = {0xfa,0x02,0x58,0x42,0x56,0x9A,0xAC,0xA0,0xA6,0xB8,0xAE,0xC5,0xD4,0xCC,0x8F,0x9B,0x8F,0xB6,0xBF,0xB6,0x00,0x9F,0x00,0x98,0x00,0xB6};
char Gamma_Set100[26] = {0xfa,0x02,0x58,0x42,0x56,0xA0,0xB0,0xA6,0xA1,0xB5,0xA9,0xC7,0xD6,0xCE,0x90,0x9C,0x90,0xB8,0xC0,0xB8,0x00,0x9B,0x00,0x94,0x00,0xB0};
char Gamma_Set90[26] = {0xfa,0x02,0x58,0x42,0x56,0xA7,0xB5,0xAC,0xA0,0xB5,0xA8,0xC7,0xD7,0xCE,0x8F,0x9B,0x90,0xB9,0xC1,0xB9,0x00,0x96,0x00,0x8F,0x00,0xAB};
char Gamma_Set80[26] = {0xfa,0x02,0x58,0x42,0x56,0x99,0xA3,0x9C,0xA1,0xB5,0xA9,0xC2,0xD5,0xC9,0x91,0x9C,0x92,0xBB,0xC3,0xBB,0x00,0x90,0x00,0x8A,0x00,0xA4};
char Gamma_Set70[26] = {0xfa,0x02,0x58,0x42,0x56,0x9F,0xA8,0xA2,0xA1,0xB6,0xAA,0xC1,0xD5,0xC7,0x91,0x9C,0x93,0xBB,0xC4,0xBB,0x00,0x8B,0x00,0x85,0x00,0x9E};
#endif
char Gamma_Set190[26] = {0xfa,0x02,0x58,0x42,0x56,0xA7,0xBF,0xAA,0xB2,0xC1,0xB9,0xBC,0xC7,0xBE,0x91,0x9F,0x90,0xAD,0xB5,0xAB,0x00,0xC5,0x00,0xB8,0x00,0xE3};
char Gamma_Set180[26] = {0xfa,0x02,0x58,0x42,0x56,0xAB,0xC2,0xAE,0xB3,0xC1,0xB9,0xBB,0xC6,0xBE,0x91,0x9F,0x90,0xAE,0xB5,0xAB,0x00,0xC1,0x00,0xB5,0x00,0xDE};
char Gamma_Set170[26] = {0xfa,0x02,0x58,0x42,0x56,0xAF,0xC5,0xB2,0xB1,0xC0,0xB6,0xBD,0xC8,0xC1,0x90,0x9E,0x8F,0xAF,0xB7,0xAD,0x00,0xBD,0x00,0xB1,0x00,0xD9};
char Gamma_Set160[26] = {0xfa,0x02,0x58,0x42,0x56,0xA2,0xB7,0xA5,0xB3,0xC2,0xB9,0xC0,0xCA,0xC4,0x90,0x9E,0x8F,0xB0,0xB8,0xAF,0x00,0xB8,0x00,0xAC,0x00,0xD3};
char Gamma_Set150[26] = {0xfa,0x02,0x58,0x42,0x56,0xA6,0xBA,0xA9,0xB1,0xC1,0xB6,0xC2,0xCC,0xC7,0x8E,0x9D,0x8D,0xB2,0xBA,0xB1,0x00,0xB4,0x00,0xA9,0x00,0xCF};
char Gamma_Set140[26] = {0xfa,0x02,0x58,0x42,0x56,0xAA,0xBE,0xAC,0xB1,0xC1,0xB6,0xBF,0xC9,0xC5,0x90,0x9F,0x8F,0xB3,0xBB,0xB3,0x00,0xAF,0x00,0xA4,0x00,0xC9};
char Gamma_Set130[26] = {0xfa,0x02,0x58,0x42,0x56,0xA2,0xB3,0xA4,0xAD,0xBF,0xB2,0xC1,0xCB,0xC8,0x90,0x9F,0x8F,0xB5,0xBD,0xB4,0x00,0xAB,0x00,0x9F,0x00,0xC3};
char Gamma_Set120[26] = {0xfa,0x02,0x58,0x42,0x56,0xA2,0xB3,0xA4,0xB0,0xC1,0xB4,0xC6,0xD0,0xCD,0x8E,0x9E,0x8C,0xB7,0xBF,0xB7,0x00,0xA6,0x00,0x9B,0x00,0xBD};
char Gamma_Set110[26] = {0xfa,0x02,0x58,0x42,0x56,0xA7,0xB7,0xA9,0xAF,0xC0,0xB2,0xC5,0xCF,0xCC,0x8E,0x9E,0x8D,0xB7,0xBF,0xB7,0x00,0xA2,0x00,0x97,0x00,0xB8};
char Gamma_Set100[26] = {0xfa,0x02,0x58,0x42,0x56,0x9E,0xAC,0xA0,0xAB,0xBD,0xAE,0xC7,0xD1,0xCE,0x90,0x9F,0x8F,0xB8,0xC0,0xB8,0x00,0x9D,0x00,0x92,0x00,0xB3};
char Gamma_Set90[26] = {0xfa,0x02,0x58,0x42,0x56,0xA4,0xB0,0xA6,0xA9,0xBD,0xAC,0xC6,0xD0,0xCE,0x91,0xA1,0x91,0xB9,0xC1,0xB9,0x00,0x98,0x00,0x8E,0x00,0xAD};
char Gamma_Set80[26] = {0xfa,0x02,0x58,0x42,0x56,0x95,0x9F,0x97,0xAA,0xBE,0xAD,0xC5,0xD0,0xCC,0x90,0xA0,0x91,0xB9,0xC2,0xB9,0x00,0x93,0x00,0x89,0x00,0xA8};
char Gamma_Set70[26] = {0xfa,0x02,0x58,0x42,0x56,0x9B,0xA3,0x9C,0xAB,0xBF,0xAD,0xC4,0xCF,0xCA,0x90,0xA0,0x92,0xBB,0xC4,0xBB,0x00,0x8D,0x00,0x84,0x00,0xA1};

#endif



char Etc_Cond_Set1_0[3] = {0xF0,0x5A,0x5A};
char Etc_Cond_Set1_1[3] = {0xF1,0x5A,0x5A};
char Etc_Cond_Set1_2[3] = {0xFC,0x5A,0x5A};

char Panel_Cond_Set[14] = {0xF8,0x27,0x27,0x08,0x08,0x4E,0xAA,0x5E,0x8A,0x10,0x3F,0x10,0x10,0x00};
char Gamma_Set[26]      = {0xfa,0x02,0x58,0x42,0x56,0x9B,0xA3,0x9C,0xAB,0xBF,0xAD,0xC4,0xCF,0xCA,0x90,0xA0,0x92,0xBB,0xC4,0xBB,0x00,0x8D,0x00,0x84,0x00,0xA1};


char Gamma_Set_Eable[2] = {0xFA,0x03};

char Etc_cond_Set2_0[4] = {0xF6,0x00,0x84,0x09};

char Etc_cond_Set2_1[2] = {0xB0,0x09};
char Etc_cond_Set2_2[2] = {0xD5,0x64};
char Etc_cond_Set2_3[2] = {0xB0,0x0B};

char Etc_cond_Set2_4[4] = {0xD5,0xA4,0x7E,0x20};

char Etc_cond_Set2_5[2] = {0xB0,0x08};
char Etc_cond_Set2_6[2] = {0xFD,0xF8};
char Etc_cond_Set2_7[2] = {0xB0,0x04};
char Etc_cond_Set2_8[2] = {0xF2,0x4D};
char Etc_cond_Set2_9[2] = {0xB0,0x05};
char Etc_cond_Set2_10[2]= {0xFD,0x1F};

char Etc_cond_Set2_11[4]= {0xb1,0x01,0x00,0x16};
char Etc_cond_Set2_12[5]= {0xb2,0x15,0x15,0x15,0x15};
char Etc_cond_Set2_13[1]= {0x11};//0x05

char Memory_Window_Set2_0[1] = {0x35};
char Memory_Window_Set2_1[5] = {0x2a,0x00,0x1e,0x02,0x39};
char Memory_Window_Set2_2[5] = {0x2b,0x00,0x00,0x03,0xbf};
char Memory_Window_Set2_3[2] = {0xd1,0x8a};

char dis_on[1] 			= {0x29};//sleep_out

char dis_off[1] 		= {0x28};
char sleep_in[1]		= {0x10};
//char acl_on[2] 		    = {0xc0,0x01};
//char gpara[3] 		    = {0xB0,0xc0,0x0C};
#ifdef CONFIG_ACL_FOR_AMOLED
char acl_on[2] 		    = {0xc0,0x01};
char acl_off[2] 		= {0xc0,0x00};

#if 0
char acl_data10[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x06,0x07,0x09,0x0A,0x0C,0x0D,0x0F,0x10};
char acl_data20[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x07,0x0B,0x0E,0x11,0x13,0x16,0x1A,0x1C,0x1F};
char acl_data30[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x0B,0x10,0x14,0x19,0x1D,0x22,0x27,0x2B,0x30};
char acl_data40[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x0E,0x15,0x1B,0x21,0x27,0x2E,0x34,0x3B,0x41};
char acl_data50[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x09,0x11,0x1A,0x22,0x2A,0x32,0x3A,0x44,0x4C,0x54};
char acl_data60[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x0B,0x15,0x20,0x2A,0x35,0x3F,0x4A,0x56,0x60,0x6B};
char acl_data70[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x0D,0x19,0x26,0x32,0x3F,0x4C,0x59,0x69,0x77,0x85};
#endif
char acl_data10[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0A,0x11,0x11,0x00};
char acl_data30[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x0D,0x14,0x1C,0x23,0x2C,0x2C,0x00};
char acl_data50[29] = {0xc1,0x47,0x53,0x13,0x53,0x00,0x00,0x01,0xDF,0x00,0x00,0x03,0x1F ,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x1C,0x28,0x36,0x43,0x51,0x5E,0x5E,0x00};

static struct dsi_cmd_desc oscar_acl_control10[]=
{
	
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(acl_data10), acl_data10}
};


static struct dsi_cmd_desc oscar_acl_control30[]=
{
	
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(acl_data30), acl_data30}
};


static struct dsi_cmd_desc oscar_acl_control50[]=
{

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(acl_data50), acl_data50}
};

static struct dsi_cmd_desc oscar_acl_on[]=
{
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(acl_on), acl_on}
};

static struct dsi_cmd_desc oscar_acl_off[]=
{
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(acl_off), acl_off}

};

#endif
#if 0
char acl_data[29]       = {0xc1,0x47,0x53,0x13,
						   0x53,0x00,0x00,0x01,
						   0xdf,0x00,0x00,0x03,
						   0x1f,0x00,0x00,0x00,
					       0x00,0x00,0x01,0x02,
						   0x03,0x07,0x0e,0x14,
						   0x1c,0x24,0x2d,0x2d,
						   0x00};

#endif
#ifdef SAMSUNG_LCD_MIPI_CELL
char ID_read[1] 		= {0xD4};//sleep_out
char Cell_on[2] 		= {0xB1,0x0B};
char Cell_off[2] 		= {0xB1,0x0A};

char Cell_on300[5] 		= {0xB2};
char Cell_on200[5] 		= {0xB2};
char Cell_on160[5] 		= {0xB2};
char Cell_on90[5] 		= {0xB2};

static struct dsi_cmd_desc oscar_panel_out=
{
	DTYPE_DCS_READ, 1, 0, 0, 5, sizeof(ID_read), ID_read
};
static struct dsi_cmd_desc Cell_on_cmd[]=
{
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Cell_on), Cell_on},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
};


#endif

static struct dsi_cmd_desc oscar_sleep_in[]=
{
	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(dis_off), dis_off},
	{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(sleep_in), sleep_in}
};/*
static struct dsi_cmd_desc oscar_panel_off[]=
{

	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(dis_off), dis_off}
};*/
static struct dsi_cmd_desc oscar_sleep_out[]=
{
	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(dis_on), dis_on}
};



static struct dsi_cmd_desc oscar_display_init_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_Cond_Set1_0), Etc_Cond_Set1_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_Cond_Set1_1), Etc_Cond_Set1_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_Cond_Set1_2), Etc_Cond_Set1_2},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Panel_Cond_Set), Panel_Cond_Set},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set70), Gamma_Set70},
	
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable},
	
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_cond_Set2_0), Etc_cond_Set2_0},
	
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_1), Etc_cond_Set2_1},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_2), Etc_cond_Set2_2},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_3), Etc_cond_Set2_3},
	
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_cond_Set2_4), Etc_cond_Set2_4},
	
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_5), Etc_cond_Set2_5},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_6), Etc_cond_Set2_6},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_7), Etc_cond_Set2_7},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_8), Etc_cond_Set2_8},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_9), Etc_cond_Set2_9},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Etc_cond_Set2_10), Etc_cond_Set2_10},
	
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_cond_Set2_11), Etc_cond_Set2_11},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Etc_cond_Set2_12), Etc_cond_Set2_12},
	
	{DTYPE_DCS_WRITE, 1, 0, 0, 120, sizeof(Etc_cond_Set2_13), Etc_cond_Set2_13},

	{DTYPE_DCS_WRITE, 1, 0, 0, 0, sizeof(Memory_Window_Set2_0), Memory_Window_Set2_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Memory_Window_Set2_1), Memory_Window_Set2_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Memory_Window_Set2_2), Memory_Window_Set2_2},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Memory_Window_Set2_3), Memory_Window_Set2_3},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(acl_on), acl_on},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(acl_data10), acl_data10}
	


};
static struct dsi_cmd_desc oscar_backlight_190_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set190), Gamma_Set190},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};

static struct dsi_cmd_desc oscar_backlight_180_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set180), Gamma_Set180},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};
static struct dsi_cmd_desc oscar_backlight_170_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set170), Gamma_Set170},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};

static struct dsi_cmd_desc oscar_backlight_160_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set160), Gamma_Set160},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};
static struct dsi_cmd_desc oscar_backlight_150_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set150), Gamma_Set150},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc oscar_backlight_140_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set140), Gamma_Set140},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};
static struct dsi_cmd_desc oscar_backlight_130_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set130), Gamma_Set130},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc oscar_backlight_120_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set120), Gamma_Set120},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc oscar_backlight_110_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set110), Gamma_Set110},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc oscar_backlight_100_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set100), Gamma_Set100},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};
static struct dsi_cmd_desc oscar_backlight_90_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set90), Gamma_Set90},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};

static struct dsi_cmd_desc oscar_backlight_80_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set80), Gamma_Set80},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};
static struct dsi_cmd_desc oscar_backlight_70_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Gamma_Set70), Gamma_Set70},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};
#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
char Smart_Gamma_Set190[26];
char Smart_Gamma_Set180[26];
char Smart_Gamma_Set170[26];
char Smart_Gamma_Set160[26];
char Smart_Gamma_Set150[26];
char Smart_Gamma_Set140[26];
char Smart_Gamma_Set130[26];
char Smart_Gamma_Set120[26];
char Smart_Gamma_Set110[26];
char Smart_Gamma_Set100[26];
char Smart_Gamma_Set90[26];
char Smart_Gamma_Set80[26];
char Smart_Gamma_Set70[26];

static struct dsi_cmd_desc Smart_oscar_backlight_190_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set190), Smart_Gamma_Set190},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_180_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set180), Smart_Gamma_Set180},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};
static struct dsi_cmd_desc Smart_oscar_backlight_170_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set170), Smart_Gamma_Set170},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_160_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set160), Smart_Gamma_Set160},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on200), Cell_on200}
#endif
};
static struct dsi_cmd_desc Smart_oscar_backlight_150_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set150), Smart_Gamma_Set150},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_140_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set140), Smart_Gamma_Set140},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};
static struct dsi_cmd_desc Smart_oscar_backlight_130_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set130), Smart_Gamma_Set130},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_120_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set120), Smart_Gamma_Set120},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_110_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set110), Smart_Gamma_Set110},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_100_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set100), Smart_Gamma_Set100},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on160), Cell_on160}
#endif
};
static struct dsi_cmd_desc Smart_oscar_backlight_90_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set90), Smart_Gamma_Set90},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};

static struct dsi_cmd_desc Smart_oscar_backlight_80_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set80), Smart_Gamma_Set80},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};
static struct dsi_cmd_desc Smart_oscar_backlight_70_cmds[]=
{
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Smart_Gamma_Set70), Smart_Gamma_Set70},
	{DTYPE_DCS_WRITE1, 1, 0, 0, 0, sizeof(Gamma_Set_Eable), Gamma_Set_Eable}
#ifdef SAMSUNG_LCD_MIPI_CELL
	,{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(Cell_on90), Cell_on90}
#endif
};
#endif




#ifdef CONFIG_ACL_FOR_AMOLED
void acl_data(struct msm_fb_data_type *mfd, int data)
{
	mutex_lock(&mfd->dma->ov_mutex);	

	mdp4_dsi_cmd_dma_busy_wait(mfd);
	mdp4_dsi_blt_dmap_busy_wait(mfd);
	mipi_dsi_mdp_busy_wait(mfd);

	mipi_set_tx_power_mode(0);

	switch(data){
		case 10 : mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control10,
							ARRAY_SIZE(oscar_acl_control10));
			oscar_state.acl_data=data;
			break;
		case 30 : mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control30,
							ARRAY_SIZE(oscar_acl_control30));
			oscar_state.acl_data=data;
			break;
	

		case 50 :mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control50,
							ARRAY_SIZE(oscar_acl_control50));
			oscar_state.acl_data=data;
			break;
	}

	mipi_set_tx_power_mode(1);	
	mutex_unlock(&mfd->dma->ov_mutex);
	printk(KERN_WARNING"mipi_oscar_lcd_acl_data = %d\n",data);
}


void acl_contol(struct msm_fb_data_type *mfd, int state)
{
	mutex_lock(&mfd->dma->ov_mutex);	

	mdp4_dsi_cmd_dma_busy_wait(mfd);
	mdp4_dsi_blt_dmap_busy_wait(mfd);
	mipi_dsi_mdp_busy_wait(mfd);

	mipi_set_tx_power_mode(0);

	if(state == true){
		mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_on,
							ARRAY_SIZE(oscar_acl_on));
		oscar_state.acl_flag = true;
	}
	else{ 
		mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_off,
							ARRAY_SIZE(oscar_acl_off));
		oscar_state.acl_flag = false;
	}

	mipi_set_tx_power_mode(1);	
	mutex_unlock(&mfd->dma->ov_mutex);
	printk(KERN_WARNING"mipi_oscar_lcd_ACL = %d\n",state);
}

#endif


#ifdef SAMSUNG_LCD_MIPI_CELL

static uint32 mipi_oscar_lcd_read(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	int i;
	uint8 *lp;

	tp = &oscar_tx_buf;
	rp = &oscar_rx_buf;	
		
	lp = NULL;

	cmd = &oscar_panel_out;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd,1);


	for(i=0; i<1;i++)
	{
		lp = ((uint8 *)rp->data++);
		printk("##mipi_oscar_lcd_read = 0x%x \n",*lp);

	}
	oscar_state.id_read_flag = true;
	oscar_state.id_read_val = (int)(*lp);

	if(oscar_state.id_read_val != 0x00){
	
		for(i=1; i<5;i++)
		{
			Cell_on300[i] = oscar_state.id_read_val;
			Cell_on200[i] = oscar_state.id_read_val + 0x06;
			Cell_on160[i] = oscar_state.id_read_val + 0x08;
			Cell_on90[i]  = oscar_state.id_read_val + 0x0B;
			if(Cell_on300[1] >= 0x28)
			{
				Cell_on300[i] = 0x28;
			}
			else if(Cell_on200[1] >= 0x28)
			{
				Cell_on200[i] = 0x28;
			}
			else if(Cell_on160[1] >= 0x28)
			{
				Cell_on160[i] = 0x28;
			}
			else if(Cell_on90[1] >= 0x28)
			{
				Cell_on90[i] = 0x28;
			}

		}
		
	}
	else{
		
		for(i=1; i<5;i++)
		{
			Cell_on300[i] = 0x15;
			Cell_on200[i] = 0x1D;
			Cell_on160[i] = 0x20;
			Cell_on90[i]  = 0x25;
		}
	
	}

	for(i=0; i<5;i++){
		printk("##Cell_on300 = 0x%x \n",Cell_on300[i]);
		printk("##Cell_on200 = 0x%x \n",Cell_on200[i]);
		printk("##Cell_on160 = 0x%x \n",Cell_on160[i]);
		printk("##Cell_on90  = 0x%x \n",Cell_on90[i]);
	}
	return *lp;
}
#endif
static int mipi_oscar_lcd_on(struct platform_device *pdev)
{


	struct msm_fb_data_type *mfd;
	struct fb_info *fbi;

    ENTER_FUNC2();

	mfd = platform_get_drvdata(pdev);
	fbi = mfd->fbi;
	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;

	if (oscar_state.disp_initialized == false) {
		
		mdelay(25);
		gpio_direction_output(gpio43, 0);
	 	udelay(10);
        gpio_direction_output(gpio43, 1);  // lcd panel reset 
        mdelay(10);

		
		mipi_set_tx_power_mode(0);	
		mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_display_init_cmds,
				ARRAY_SIZE(oscar_display_init_cmds));
		mipi_set_tx_power_mode(1);	
		
#ifdef SAMSUNG_LCD_MIPI_CELL
	if(!oscar_state.id_read_flag){                 //read ID3
		mipi_set_tx_power_mode(0);
		mipi_dsi_cmd_bta_sw_trigger(); 
		mipi_oscar_lcd_read(mfd);
		mipi_set_tx_power_mode(1);
		printk("SAMSUNG_LCD_MIPI_CELL\n");
#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
		INIT_DELAYED_WORK(&oscar_state.mtp_work,mtp_work_fnc);
		schedule_delayed_work(&oscar_state.mtp_work, 2 *HZ);
#endif
	}
#endif

#ifdef CONFIG_ACL_FOR_AMOLED		
		
			mipi_set_tx_power_mode(0);
			switch(oscar_state.acl_data)
			{
				case 10: mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control10,
								ARRAY_SIZE(oscar_acl_control10));
						break;
				case 30: mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control30,
								ARRAY_SIZE(oscar_acl_control30));
				
						break;
				case 50: mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_acl_control50,
								ARRAY_SIZE(oscar_acl_control50));
				
						break;
			}
			mipi_set_tx_power_mode(1);
		
#endif	
		if(first == 0){
			mipi_set_tx_power_mode(0);
			mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_out,
				ARRAY_SIZE(oscar_sleep_out));
			mipi_set_tx_power_mode(1);
			first =true;
			
		}
		oscar_state.disp_initialized = true;
	}

#ifdef SAMSUNG_LCD_MIPI_CELL
	mipi_set_tx_power_mode(0);					//after read id, 
	mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Cell_on_cmd,
			ARRAY_SIZE(Cell_on_cmd));
	mipi_set_tx_power_mode(1);		
#endif
	oscar_state.disp_on = true;

	printk(KERN_WARNING"mipi_oscar_lcd_acl_data = %d, %d\n",oscar_state.acl_data,oscar_state.acl_data);

	EXIT_FUNC2();

	return 0;
}
void mipi_oscar_sec_lcd_on(struct msm_fb_data_type *mfd)
{

	mipi_set_tx_power_mode(0);	
 	mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_out,
			 	ARRAY_SIZE(oscar_sleep_out));
	mipi_set_tx_power_mode(1);	
		
}

static int mipi_oscar_lcd_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;

    ENTER_FUNC2();

	mfd = platform_get_drvdata(pdev);

	if (!mfd)
		return -ENODEV;
	if (mfd->key != MFD_KEY)
		return -EINVAL;

	if (oscar_state.disp_on == true) {
	
			mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_in,
				ARRAY_SIZE(oscar_sleep_in));
		
		oscar_state.disp_on = false;
		oscar_state.disp_initialized = false;
	}
    backlight_cnt = false;
	oscar_state.disp_powered_up = false;

	EXIT_FUNC2();
	return 0;

}

extern struct fb_info *registered_fb[FB_MAX]; 
//extern int mipi_dsi_cdp_panel_power(int on);
extern int mipi_dsi_cdp_panel_power_Oscar(int on);
void android_poweroff(void)
{
    struct fb_info *info; 
    struct msm_fb_data_type *mfd;

    info = registered_fb[0];
	mfd = (struct msm_fb_data_type *)info->par;
	if(mfd->panel_power_on == FALSE)
		return;
	
	mipi_set_tx_power_mode(0);	
	mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_in,
				ARRAY_SIZE(oscar_sleep_in));
	mipi_set_tx_power_mode(1);
	gpio_direction_output(gpio43, 0);
	mdelay(110);
	//mipi_dsi_cdp_panel_power(0);
	mipi_dsi_cdp_panel_power_Oscar(0);
	EXIT_FUNC2();

}
EXPORT_SYMBOL(android_poweroff);
static void mipi_oscar_set_backlight(struct msm_fb_data_type *mfd)
{

	if(!mfd)
		return;
	if(mfd->key != MFD_KEY)
		return;
	

	mutex_lock(&mfd->dma->ov_mutex);	
	//if(chargerFlag == false){
#if 0
		if (mdp4_overlay_dsi_state_get() <= ST_DSI_SUSPEND) {
			//mutex_unlock(&mfd->dma->ov_mutex);
			//return;
			mipi_dsi_turn_on_clks();
		}
#endif
	//}

	mdp4_dsi_cmd_dma_busy_wait(mfd);
	mdp4_dsi_blt_dmap_busy_wait(mfd);
	mipi_dsi_mdp_busy_wait(mfd);
	
	if(backlight_cnt == true)
	{
			mipi_set_tx_power_mode(0);
			mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_out,
				ARRAY_SIZE(oscar_sleep_out));
			mipi_set_tx_power_mode(1);
			
			backlight_cnt = false;	
	}
	
	mipi_set_tx_power_mode(0);	
	/*if(oscar_state.backlightoff == false && chargerFlag == true){
		
		mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_sleep_out,
						ARRAY_SIZE(oscar_sleep_out));
		
	}*/
	switch(mfd->bl_level){
		case 13 :
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_190_cmds,ARRAY_SIZE(Smart_oscar_backlight_190_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_190_cmds,ARRAY_SIZE(oscar_backlight_190_cmds));
				  break;
	
		case 12 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_180_cmds,ARRAY_SIZE(Smart_oscar_backlight_180_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_180_cmds,ARRAY_SIZE(oscar_backlight_180_cmds));
				
				  break;
		case 11 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_170_cmds,ARRAY_SIZE(Smart_oscar_backlight_170_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_170_cmds,ARRAY_SIZE(oscar_backlight_170_cmds));
				
				  break;
		case 10 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_160_cmds,ARRAY_SIZE(Smart_oscar_backlight_160_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_160_cmds,ARRAY_SIZE(oscar_backlight_160_cmds));
				
				  break;
		case 9 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_150_cmds,ARRAY_SIZE(Smart_oscar_backlight_150_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_150_cmds,ARRAY_SIZE(oscar_backlight_150_cmds));
			
				  break;
		case 8 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_140_cmds,ARRAY_SIZE(Smart_oscar_backlight_140_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_140_cmds,ARRAY_SIZE(oscar_backlight_140_cmds));
			
				  break;
		case 7 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_130_cmds,ARRAY_SIZE(Smart_oscar_backlight_130_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_130_cmds,ARRAY_SIZE(oscar_backlight_130_cmds));
		
				  break;
		case 6 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_120_cmds,ARRAY_SIZE(Smart_oscar_backlight_120_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_120_cmds,ARRAY_SIZE(oscar_backlight_120_cmds));
			
				  break;
		case 5 : 	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_110_cmds,ARRAY_SIZE(Smart_oscar_backlight_110_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_110_cmds,ARRAY_SIZE(oscar_backlight_110_cmds));
				  break;
		case 4 : 	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_100_cmds,ARRAY_SIZE(Smart_oscar_backlight_100_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_100_cmds,ARRAY_SIZE(oscar_backlight_100_cmds));

				  break;
		case 3 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_90_cmds,ARRAY_SIZE(Smart_oscar_backlight_90_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_90_cmds,ARRAY_SIZE(oscar_backlight_90_cmds));
				  break;
		case 2 : 	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_80_cmds,ARRAY_SIZE(Smart_oscar_backlight_80_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_80_cmds,ARRAY_SIZE(oscar_backlight_80_cmds));
				  break;
				  
		case 1 : 	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_70_cmds,ARRAY_SIZE(Smart_oscar_backlight_70_cmds));
						
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_70_cmds,ARRAY_SIZE(oscar_backlight_70_cmds));
	
				  break;
		case 0 :	
					#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
					if(chnnel_mtp)
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, Smart_oscar_backlight_70_cmds,ARRAY_SIZE(Smart_oscar_backlight_70_cmds));
					else
					#endif
						mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, oscar_backlight_70_cmds,ARRAY_SIZE(oscar_backlight_70_cmds));
					if(backlight_control == true)
						backlight_cnt = true;
				  break;
		}

	mipi_set_tx_power_mode(1);

	
	mutex_unlock(&mfd->dma->ov_mutex);
	
	if(mfd->bl_level > 0)
		oscar_state.backlightoff = true;
	else
		oscar_state.backlightoff = false;

	printk(KERN_WARNING"[%s] = %d, backlight_cnt = %d\n",__func__,mfd->bl_level,backlight_cnt);

}
#ifdef CONFIG_F_SKYDISP_SMART_DIMMING
//static struct class * oscar_lcd_panel_info;
//static struct device *lcd_panel_info_device;
char read_panel[2]	= { 0xd3, 0x00 };
char write_gpara1[2]		= { 0xB0, 0x0A };
char write_gpara2[2]		= { 0xB0, 0x14 };


static struct dsi_cmd_desc samsung_oled_hd_read_panel ={
	DTYPE_DCS_READ, 1, 0, 0, 0, sizeof(read_panel), read_panel
};

static struct dsi_cmd_desc samsung_oled_hd_gpara11_cmd[] ={
	{ DTYPE_DCS_LWRITE, 1, 0, 1, 0, sizeof(write_gpara1), write_gpara1},
};
static struct dsi_cmd_desc samsung_oled_hd_gpara21_cmd[] ={
	{ DTYPE_DCS_LWRITE, 1, 0, 1, 0, sizeof(write_gpara2), write_gpara2},
};

char mtp_buffer[30];
char gamma_buffer[13][25];
int mtp_count;
void mipi_samsung_oled_hd_read(struct msm_fb_data_type *mfd)
{
	struct dsi_buf *rp, *tp;
	struct dsi_cmd_desc *cmd;
	int i;
	uint8 *lp;

	tp = &oscar_tx_buf;
	rp = &oscar_rx_buf;	
		
	lp = NULL;

	cmd = &samsung_oled_hd_read_panel;
	mipi_dsi_cmds_rx(mfd, tp, rp, cmd,10);
	
	for(i=0; i<10; i++)
	{
		lp = ((uint8 *)rp->data++);
		
		mtp_buffer[i + mtp_count] = *lp;
		printk("[PANTECH_LCD]MTP = %d %d\n",*lp,mtp_buffer[i + mtp_count]); 
		msleep(1);
	}
	mtp_count += 10; 

}

void mtp_read(void)
{

	struct fb_info *info; 
    struct msm_fb_data_type *mfd;

    info = registered_fb[0];
	mfd = (struct msm_fb_data_type *)info->par;

	
	mipi_dsi_cmd_bta_sw_trigger(); 
	mipi_samsung_oled_hd_read(mfd);
	
	mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, samsung_oled_hd_gpara11_cmd, 
						ARRAY_SIZE(samsung_oled_hd_gpara11_cmd));
	
	mipi_dsi_cmd_bta_sw_trigger(); 
	mipi_samsung_oled_hd_read(mfd);
	
	
	mipi_dsi_cmds_tx(mfd, &oscar_tx_buf, samsung_oled_hd_gpara21_cmd, 
						ARRAY_SIZE(samsung_oled_hd_gpara21_cmd));
	
	mipi_dsi_cmd_bta_sw_trigger(); 
	mipi_samsung_oled_hd_read(mfd);

	mtp_buffer[6] = mtp_buffer[6] & 0x01;
	mtp_buffer[14] = mtp_buffer[14] & 0x01;
	mtp_buffer[22] = mtp_buffer[22] & 0x01;

}



void mtp_write(void)
{
	int ii,jj;

	for(jj=0;jj<25;jj++){
		Smart_Gamma_Set190[jj+1] = gamma_buffer[0][jj];
		Smart_Gamma_Set180[jj+1] = gamma_buffer[1][jj];
		Smart_Gamma_Set170[jj+1] = gamma_buffer[2][jj];
		Smart_Gamma_Set160[jj+1] = gamma_buffer[3][jj];
		Smart_Gamma_Set150[jj+1] = gamma_buffer[4][jj];
		Smart_Gamma_Set140[jj+1] = gamma_buffer[5][jj];
		Smart_Gamma_Set130[jj+1] = gamma_buffer[6][jj];
		Smart_Gamma_Set120[jj+1] = gamma_buffer[7][jj];
		Smart_Gamma_Set110[jj+1] = gamma_buffer[8][jj];
		Smart_Gamma_Set100[jj+1] = gamma_buffer[9][jj];
		Smart_Gamma_Set90[jj+1] = gamma_buffer[10][jj];
		Smart_Gamma_Set80[jj+1] = gamma_buffer[11][jj];
		Smart_Gamma_Set70[jj+1] = gamma_buffer[12][jj];				
	}

	Smart_Gamma_Set190[0] = 0xfa;
	Smart_Gamma_Set180[0] = 0xfa;
	Smart_Gamma_Set170[0] = 0xfa;
	Smart_Gamma_Set160[0] = 0xfa;
	Smart_Gamma_Set150[0] = 0xfa;
	Smart_Gamma_Set140[0] = 0xfa;
	Smart_Gamma_Set130[0] = 0xfa;
	Smart_Gamma_Set120[0] = 0xfa;
	Smart_Gamma_Set110[0] = 0xfa;
	Smart_Gamma_Set100[0] = 0xfa;
	Smart_Gamma_Set90[0] = 0xfa;
	Smart_Gamma_Set80[0] = 0xfa;
	Smart_Gamma_Set70[0] = 0xfa;
	chnnel_mtp=true;

	for(ii = 0;ii < 13;ii++){
		printk(KERN_WARNING"\n");
		for(jj=0;jj<25;jj++)
			printk(KERN_WARNING"[%s]gamma val = 0x%x\n",__func__,gamma_buffer[ii][jj]);
	}
	printk(KERN_WARNING"[%s]Smart Dimming 0n = %d\n",__func__,chnnel_mtp);
}
void mtp_work_fnc(struct work_struct *work)
{

	mtp_read();

}
#endif
static int __devinit mipi_oscar_lcd_probe(struct platform_device *pdev)
{
	
	
	if (pdev->id == 0) {
        mipi_oscar_pdata = pdev->dev.platform_data;
		return 0;
	}

	msm_fb_add_device(pdev);

	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_oscar_lcd_probe,
	.driver = {
		.name   = "mipi_oscar",
	},
};

static struct msm_fb_panel_data oscar_panel_data = {
       .on             = mipi_oscar_lcd_on,
       .off            = mipi_oscar_lcd_off,
       .set_backlight  = mipi_oscar_set_backlight,
};

static int ch_used[3];

int mipi_oscar_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel)
{
	struct platform_device *pdev = NULL;
	int ret;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_oscar", (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	oscar_panel_data.panel_info = *pinfo;

	ret = platform_device_add_data(pdev, &oscar_panel_data,
		sizeof(oscar_panel_data));
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

static int __init mipi_oscar_lcd_init(void)
{
    ENTER_FUNC2();

    oscar_state.disp_powered_up = true;

    mipi_dsi_buf_alloc(&oscar_tx_buf, DSI_BUF_SIZE);
    mipi_dsi_buf_alloc(&oscar_rx_buf, DSI_BUF_SIZE);

    EXIT_FUNC2();

    return platform_driver_register(&this_driver);

}

module_init(mipi_oscar_lcd_init);

