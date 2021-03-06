/*
 * Gadget Driver for Android USBNET
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __F_USBNET_H
#define __F_USBNET_H

int usbnet_function_add(struct usb_composite_dev *cdev,
	struct usb_configuration *c);

struct usb_function *usbnet_function_enable(int enable, int id);

#endif /* __F_USBNET_H */
