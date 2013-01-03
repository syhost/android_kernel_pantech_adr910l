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
#include <linux/mfd/pmic8058.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/workqueue.h>
#include <linux/miscdevice.h>
#include <linux/felica.h>
#include "felica_master.h"

#define PRT_NAME "snfc_hsel"
#define HSEL_LOW  0x0
#define HSEL_HIGH 0x1

/**
 * @brief   Open file operation of Snfc HSEL controller
 * @param   inode : (unused)
 * @param   file  : (unused)
 * @retval  0     : Success
 * @note
 */
static int snfc_hsel_open(struct inode *inode, struct file *file)
{
	pr_debug(PRT_NAME ": %s\n", __func__);

	return 0;
}

/**
 * @brief   Close file operation of Snfc HSEL controller
 * @details This module is responsible for the following roles:\n
 *            # Forcedly, write Low to PON GPIO\n
 *            # Forcedly, turn off TVDD
 * @param   inode    : (unused)
 * @param   file     : (unused)
 * @retval  0        : Success
 * @note
 */
static int snfc_hsel_release(struct inode *inode, struct file *file)
{
	struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;
	pr_debug(PRT_NAME ": %s\n", __func__);

	/* Forcedly, write Low to PON GPIO */
	gpio_set_value(pfdata->gpio_pon, HSEL_LOW);

	return 0;
}

/**
 * @brief   Write file operation of Snfc HSEL controller
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
static ssize_t snfc_hsel_write(struct file *file, const char __user *buf,
					size_t count, loff_t *offset)
{
	char kbuf;
	struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	if (1 != count  || !buf) {
		pr_err(PRT_NAME ": Error. Invalid arg @PON write.\n");
		return -EINVAL;
	}

	/* Copy value from user space */
	if (copy_from_user(&kbuf, buf, 1)) {
		pr_err(PRT_NAME ": Error. copy_from_user failure.\n");
		return -EFAULT;
	}

	if (HSEL_HIGH == kbuf) {
		/* Write High to HSEL GPIO */
		gpio_set_value(pfdata->gpio_hsel, HSEL_HIGH);
	} else if (HSEL_LOW == kbuf) {
		/* Write LOW to HSEL GPIO */
		gpio_set_value(pfdata->gpio_hsel, HSEL_LOW);
	} else {
		pr_err(PRT_NAME ": Error. Invalid val @HSEL write.\n");
		return -EINVAL;
	}

	/* 1 byte write */
	return 1;
}

/***************** HSEL FOPS ****************************/
static const struct file_operations snfc_hsel_fops = {
	.owner		= THIS_MODULE,
	.write		= snfc_hsel_write,
	.open		= snfc_hsel_open,
	.release	= snfc_hsel_release,
};

static struct miscdevice snfc_hsel_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "snfc_hsel",
	.fops = &snfc_hsel_fops,
};

/**
 * @brief  Initilialize Snfc HSEL controller
 * @details This function executes;\n
 *            # Check HSEL platform data\n
 *            # Alloc and init HSEL controller's data\n
 *            # Request PON GPIO\n
 *            # Disable TVDD vreg device\n
 *            # Create PON character device (/dev/snfc_hsel)
 * @param   pfdata   : Pointer to PON platform data
 * @retval  0        : Success
 * @retval  Negative : Initialization failed.\n
 *            -EINVAL = No platform data\n
 *            -ENODEV = Requesting GPIO failed / Getting TVDD dev failed\n
 *            -ENOMEM = No enough memory / Cannot create char dev
 *            -EIO    = Cannot control VREG
 * @note
 */
int snfc_hsel_probe_func(struct felica_device *pdevice)
{
	int ret;
	struct felica_platform_data *pfdata= pdevice->pdev->dev.platform_data;
	
	pr_debug(PRT_NAME ": %s\n", __func__);

	/* Check PON platform data */
	if (!pfdata) {
		pr_err(PRT_NAME ": Error. No platform data for PON.\n");
		return -EINVAL;
	}

	/* Request PON GPIO */
	ret = gpio_request(pfdata->gpio_hsel, "snfc_hsel");
	if (ret) {
		pr_err(PRT_NAME ": Error. PON GPIO request failed.\n");
		ret = -ENODEV;
		goto err_request_hsel_gpio;
	}
	ret = gpio_direction_output(pfdata->gpio_hsel, HSEL_LOW); // temp
	//ret = gpio_direction_output(pfdata->gpio_hsel, HSEL_HIGH);
	if (ret) {
		pr_err(PRT_NAME ": Error. HSEL GPIO direction failed.\n");
		ret = -ENODEV;
		goto err_direction_hsel_gpio;
	}

	/* Create PON character device (/dev/felica_pon) */
	if (misc_register(&snfc_hsel_device)) {
		pr_err(PRT_NAME ": Error. Cannot register PON.\n");
		ret = -ENOMEM;
		goto err_create_hsel_dev;
	}

	return 0;

err_create_hsel_dev:
err_direction_hsel_gpio:
	gpio_free(pfdata->gpio_hsel);
err_request_hsel_gpio:
	return ret;
}

/**
 * @brief   Terminate Snfc HSEL controller
 * @details This function executes;\n
 *            # Deregister HSEL character device (/dev/snfc_hsel)\n
 *            # Release HSEL GPIO resource\n
 *            # Release HSEL controller's data
 * @param   N/A
 * @retval  N/A
 * @note
 */

void snfc_hsel_remove_func(void)
{
	struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	misc_deregister(&snfc_hsel_device);
	gpio_free(pfdata->gpio_hsel);
}

