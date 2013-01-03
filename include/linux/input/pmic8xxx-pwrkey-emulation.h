/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __PMIC8XXX_PWRKEY_EMULATION_H__
#define __PMIC8XXX_PWRKEY_EMULATION_H__

#define PM8XXX_PWRKEY_EMULATION_DEV_NAME "pm8xxx-pwrkey-emulation"

/**
 * struct pm8xxx_pwrkey_emulation_platform_data - platform data for pwrkey emulation driver
 * @trigger_delay_ms: time delay for power key state change emulation
 *                  trigger.
 */
struct pm8xxx_pwrkey_emulation_platform_data  {
	u32  trigger_delay_ms;
};

#endif /* __PMIC8XXX_PWRKEY_EMULATION_H__ */

