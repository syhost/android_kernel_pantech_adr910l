/* vendor/semc/hardware/felica/felica_rfs.c
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
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/felica.h>
#include "felica_master.h"

#define PRT_NAME "felica rfs"
#define RFS_GPIO_L 0
#define RFS_GPIO_H 1
#define RFS_VAL_L  0x00
#define RFS_VAL_H  0x01


/**
 * @brief   Open file operation of FeliCa RFS controller
 * @param   inode : (unused)
 * @param   file  : (unused)
 * @retval  0     : Success
 * @note
 */
static int felica_rfs_open(struct inode *inode, struct file *file)
{
	pr_debug(PRT_NAME ": %s\n", __func__);

	return 0;
}

/**
 * @brief   Close file operation of FeliCa RFS controller
 * @param   inode : (unused)
 * @param   file  : (unused)
 * @retval  0     : Success
 * @note
 */
static int felica_rfs_release(struct inode *inode, struct file *file)
{
	pr_debug(PRT_NAME ": %s\n", __func__);

	return 0;
}

/**
 * @brief   Read file operation of FeliCa RFS controller
 * @details This function executes;\n
 *            # Read RFS GPIO value\n
 *            # Copy the value to user space
 * @param   inode    : (unused)
 * @param   file     : (unused)
 * @retval  1        : Success
 * @retval  Negative : Failure\n
 *            -EINVAL = Invalid argument\n
 *            -EIO    = GPIO read error\n
 *            -EFAULT = Cannot copy data to user space
 * @note
 */
static ssize_t felica_rfs_read(struct file *file, char __user *buf,
					size_t count, loff_t *offset)
{
	char kbuf;
	int gpio_val;
	struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	if (1 != count  || !buf) {
		pr_err(PRT_NAME ": Error. Invalid arg @RFS read.\n");
		return -EINVAL;
	}

	/* Read RFS GPIO value */
	gpio_val = gpio_get_value(pfdata->gpio_rfs);
	if (RFS_GPIO_L == gpio_val)
		kbuf = RFS_VAL_H;
	else if (RFS_GPIO_H == gpio_val)
		kbuf = RFS_VAL_L;
	else {
		pr_err(PRT_NAME ": Error. Invalid GPIO value @RFS read.\n");
		return -EIO;
	}

	/* Copy the value to user space */
	if (copy_to_user(buf, &kbuf, 1)) {
		pr_err(PRT_NAME ": Error. copy_to_user failure.\n");
		return -EFAULT;
	}

	/* 1 byte read */
	return 1;
}

/***************** RFS FOPS ****************************/
static const struct file_operations felica_rfs_fops = {
	.owner		= THIS_MODULE,
	.read		= felica_rfs_read,
	.open		= felica_rfs_open,
	.release	= felica_rfs_release,
};

static struct miscdevice felica_rfs_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "felica_rfs",
	.fops = &felica_rfs_fops,
};

/**
 * @brief  Initilialize FeliCa RFS controller
 * @details This function executes;\n
 *            # Check RFS platform data\n
 *            # Alloc and init RFS controller's data\n
 *            # Request RFS GPIO\n
 *            # Create RFS character device (/dev/felica_rfs)
 * @param   pfdata   : Pointer to RFS platform data
 * @retval  0        : Success
 * @retval  Negative : Initialization failed.\n
 *            -EINVAL = No platform data\n
 *            -ENODEV = Requesting GPIO failed\n
 *            -ENOMEM = No enough memory / Cannot create char dev
 * @note
 */
int felica_rfs_probe_func(struct felica_device *pdevice)
{
	int ret;
	struct felica_platform_data *pfdata= pdevice->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	/* Check RFS platform data */
	if (!pfdata) {
		pr_err(PRT_NAME ": Error. No platform data for RFS.\n");
		return -EINVAL;
	}

	
	/* Request RFS GPIO */
	ret = gpio_request(pfdata->gpio_rfs, "felica_rfs");
	if (ret) {
		pr_err(PRT_NAME ": Error. RFS GPIO request failed.\n");
		ret = -ENODEV;
		goto err_request_rfs_gpio;
	}
	ret = gpio_direction_input(pfdata->gpio_rfs);
	if (ret) {
		pr_err(PRT_NAME ": Error. RFS GPIO direction failed.\n");
		ret = -ENODEV;
		goto err_direction_rfs_gpio;
	}

	/* Create RFS character device (/dev/felica_rfs) */
	if (misc_register(&felica_rfs_device)) {
		pr_err(PRT_NAME ": Error. Cannot register RFS.\n");
		ret = -ENOMEM;
		goto err_create_rfs_dev;
	}

	return 0;

err_create_rfs_dev:
err_direction_rfs_gpio:
	gpio_free(pfdata->gpio_rfs);
err_request_rfs_gpio:
	return ret;
}

/**
 * @brief   Terminate FeliCa RFS controller
 * @details This function executes;\n
 *            # Deregister RFS character device (/dev/felica_rfs)\n
 *            # Release RFS GPIO resource\n
 *            # Release RFS controller's data
 * @param   N/A
 * @retval  N/A
 * @note
 */
void felica_rfs_remove_func(void)
{
	struct felica_platform_data *pfdata= felica_drv->pdev->dev.platform_data;

	pr_debug(PRT_NAME ": %s\n", __func__);

	misc_deregister(&felica_rfs_device);
	gpio_free(pfdata->gpio_rfs);
}
