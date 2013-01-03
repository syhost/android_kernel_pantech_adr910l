/************************************************************************************************
**
**    PANTECH AUDIO
**
**    FILE
**        pantech_audio.c
**
**    DESCRIPTION
**        This file contains pantech audio apis
**
**    Copyright (c) 2012 by PANTECH Incorporated.  All Rights Reserved.
*************************************************************************************************/

/************************************************************************************************
** Includes
*************************************************************************************************/
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <mach/gpio.h>
#include <asm/ioctls.h>
#include <linux/uaccess.h>

#include "pantech_aud_ctl.h"
#include "q6voice.h"

/*==========================================================================
** pantech_audio_ioctl
**=========================================================================*/

#ifdef CONFIG_MACH_MSM8960_STARQ
int g_pre_nr_cmd = 0;
#endif

static long pantech_audio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
#ifdef CONFIG_MACH_MSM8960_STARQ  // NR on-off apr: jykim120217@LS1
	int data = 0;

	//printk("[SKY SND] pantech_audio_ioctl, cmd=%i\n", cmd);

	switch (cmd) {
		case PANTECH_AUDIO_NR_CTL: {
			if (copy_from_user(&data, (void __user *)arg, sizeof(data))) {
				ret = -1;
				break;
			}
#if 1
			printk("[SKY SND] PANTECH_AUDIO_NR_CTL, data=0x%X\n", data);
#else
			if  (g_pre_nr_cmd == data) {
				//printk("[SKY SND] PANTECH_AUDIO_NR_CTL: Ignore duplicate NR command\n");
				break;
			} else {
				printk("[SKY SND] PANTECH_AUDIO_NR_CTL, data=0x%X\n", data);
			}
#endif

			ret = voice_send_set_oempp_enable_cmd(data);
			if (ret < 0) {
				pr_err("voice_send_set_oempp_enable_cmd failed\n");
				break;
			} else {
				g_pre_nr_cmd = data;
			}
			break;
		}
		
		default: {
			printk("\n--------------- INVALID COMMAND ---------------");
			ret = -1;
			break;
		}
	}
#endif	// CONFIG_MACH_MSM8960_STARQ
	return ret;
}

/*==========================================================================
** pantech_audio_open
**=========================================================================*/

static int pantech_audio_open(struct inode *inode, struct file *file)
{
	//printk("aud_sub_open");
	return 0;
}

/*==========================================================================
** pantech_audio_release
**=========================================================================*/

static int pantech_audio_release(struct inode *inode, struct file *file)
{
	//printk("aud_sub_release");
	return 0;	
}

/*=========================================================================*/

static struct file_operations snd_fops = {
	.owner = THIS_MODULE,
	.open = pantech_audio_open,
	.release = pantech_audio_release,
	.unlocked_ioctl	= pantech_audio_ioctl,
};

struct miscdevice pantech_audio_misc = 
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pantech_aud_ctl",
	.fops = &snd_fops
};

/*==========================================================================
** pantech_audio_init
**=========================================================================*/

static int __init pantech_audio_init(void)
{
	int result = 0;

	result = misc_register(&pantech_audio_misc);
	if(result)
	{
		printk("pantech_audio_init: misc_register failed\n");
	}

	return result;
}

/*==========================================================================
** pantech_audio_exit
**=========================================================================*/

static void __exit pantech_audio_exit(void)
{
}

/*=========================================================================*/

module_init(pantech_audio_init);
module_exit(pantech_audio_exit);

MODULE_DESCRIPTION("Pantech audio driver");
MODULE_LICENSE("GPL v2");

/*=========================================================================*/
