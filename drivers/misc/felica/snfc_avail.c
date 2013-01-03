/* vendor/semc/hardware/felica/felica_int.c
 *
 * Copyright (C) 2010 Sony Ericsson Mobile Communications AB.
 *
 * Author: Hiroaki Kuriyama <Hiroaki.Kuriyama@sonyericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/felica.h>
#include "felica_master.h"

#define PRT_NAME "snfc_avail"
#define AVAIL_LOW  0x0
#define AVAIL_HIGH 0x1

/**
 * @brief   Open file operation of Snfc Avail controller
 * @param   inode : (unused)
 * @param   file  : (unused)
 * @retval  0     : Success
 * @note
 */
static int snfc_avail_open(struct inode *inode, struct file *file)
{
	pr_debug(PRT_NAME ": %s\n", __func__);

	return 0;
}

/**
 * @brief   Close file operation of Snfc Avail controller
 * @details This module is responsible for the following roles:\n
 *            # Forcedly, write Low to PON GPIO\n
 *            # Forcedly, turn off TVDD
 * @param   inode    : (unused)
 * @param   file     : (unused)
 * @retval  0        : Success
 * @note
 */
static int snfc_avail_release(struct inode *inode, struct file *file)
{
	pr_debug(PRT_NAME ": %s\n", __func__);

	return 0;
}

/**
 * @brief   Write file operation of Snfc Avail controller
 * @details This function executes;\n
 *            # Copy value from user space\n
 *            # [When writing High,]\n
 *            #   | Write High to PON GPIO\n
 *            #   | [Params meet the condition,] turn on TVDD.\n
 *            # [When writing Low,]\n
 *            #   | Write Low to PON GPIO\n
 *            #   | Forcedly, turn off TVDD.\n
 * @param   file     : (unused)
 * @param   buf      : Source of the written data
 * @param   count    : Data length must be 1 Byte.
 * @param   offset   : (unused)
 * @retval  1        : Success
 * @retval  Negative : Failure\n
 *            -EINVAL = Invalid argument\n
 *            -EFAULT = Cannot copy data from user space\n
 *            -EIO    = Cannot control VREG
 * @note
 */
static ssize_t snfc_avail_read(struct file *file, char __user *buf,
					size_t count, loff_t *offset)
{
	char kbuf;
	//struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	if (1 != count  || !buf) {
		pr_err(PRT_NAME ": Error. Invalid arg @PON write.\n");
		return -EINVAL;
	}

	/* Temp Write High  */
	kbuf = AVAIL_HIGH;
	
	/* Copy the value to user space */
	if (copy_to_user(buf, &kbuf, 1)) {
		pr_err(PRT_NAME ": Error. copy_to_user failure.\n");
		return -EFAULT;
	}

	/* 1 byte write */
	return 1;
}

/***************** AVAIL FOPS ****************************/
static const struct file_operations snfc_avail_fops = {
	.owner		= THIS_MODULE,
	.read		= snfc_avail_read,
	.open		= snfc_avail_open,
	.release	= snfc_avail_release,
};

static struct miscdevice snfc_avail_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "snfc_avail",
	.fops = &snfc_avail_fops,
};


/**
 * @brief   Initialize Snfc Avail controller
 * @details This function executes;\n
 *            # Check INT platform data\n
 *            # Alloc and init INT controller's data\n
 *            # Init work of FeliCa push\n
 *            # Request INT GPIO\n
 *            # Request IRQ for INT GPIO\n
 *            # Enable IRQ wake for INT GPIO\n
 *            # Create INT switch device (felica_push)\n
 *            # Set default state of the device
 * @param   pfdata   : Pointer to INT platform data
 * @retval  0        : Success
 * @retval  Negative : Failure\n
 *            -EINVAL = No platform data\n
 *            -ENOMEM = No enough memory / Cannot create switch dev\n
 *            -ENODEV = GPIO request failed\n
 *            -EIO    = IRQ request failed / Enabling IRQ wake failed
 */
int snfc_avail_probe_func(struct felica_device *pdevice)
{
	int ret=0;
	struct felica_platform_data *pfdata= pdevice->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	/* Check INT platform data */
	if (!pfdata) {
		pr_err(PRT_NAME ": Error. No platform data for INT.\n");
		return -EINVAL;
	}

	
	/* Create PON character device (/dev/snfc_avail) */
	if (misc_register(&snfc_avail_device)) {
		pr_err(PRT_NAME ": Error. Cannot register PON.\n");
		ret = -ENOMEM;
	}

	return ret;
}

/**
 * @brief   Terminate Snfc Avail controller
 * @details This function executes;\n
 *            # Unregister switch device (felica_push)\n
 *            # Release IRQ for INT GPIO\n
 *            # Release INT GPIO resource\n
 *            # Release INT controller's data
 * @param   N/A
 * @retval  N/A
 * @note
 */
void snfc_avail_remove_func(void)
{
	pr_debug(PRT_NAME ": %s\n", __func__);
	misc_deregister(&snfc_avail_device);
}
