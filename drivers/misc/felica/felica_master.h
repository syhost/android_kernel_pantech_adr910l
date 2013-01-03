/* nfc/felica_master_int.h
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

#ifndef _FELICA_MASTER_H
#define _FELICA_MASTER_H
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/spinlock_types.h>
#include <linux/types.h>

struct felica_device {
	struct platform_device *pdev;
	
 	struct work_struct      felica_work_int;

	struct mutex            snfc_intu_mutex;
	wait_queue_head_t       snfc_intu_wq;
	
	spinlock_t              felica_lock_int;
	spinlock_t              snfc_lock_intu;
	bool                    snfc_irq_enabled;
	
	atomic_t                pon_status;
	atomic_t                rws_status;
	atomic_t                cen_status;
	atomic_t                rfs_status;
	atomic_t                int_status;
	atomic_t                intu_status;
	atomic_t                hsel_status;
	atomic_t                avail_status;
};

extern struct felica_device *felica_drv;

int felica_cen_probe_func(struct felica_device *);
void felica_cen_remove_func(void);

int felica_int_probe_func(struct felica_device *);
void felica_int_remove_func(void);

int felica_pon_probe_func(struct felica_device *);
void felica_pon_remove_func(void);

int felica_rfs_probe_func(struct felica_device *);
void felica_rfs_remove_func(void);

int felica_rws_probe_func(void);
void felica_rws_remove_func(void);

int snfc_intu_probe_func(struct felica_device *pdevice);
void snfc_intu_remove_func(void);

int snfc_hsel_probe_func(struct felica_device *pdevice);
void snfc_hsel_remove_func(void);

int snfc_avail_probe_func(struct felica_device *pdevice);
void snfc_avail_remove_func(void);
#endif
