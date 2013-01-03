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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/log2.h>
#include <linux/workqueue.h>

#include <linux/mfd/pm8xxx/core.h>
#include <linux/input/pmic8xxx-pwrkey-emulation.h>

#include <linux/delay.h>

/**
 * struct pmic8xxx_pwrkey_emulation - pmic8xxx pwrkey emulation information
 * @power_key_emualtion_func: key press emulation func
 * @pdata: platform data
 */
struct pmic8xxx_pwrkey_emulation {
	struct input_dev *pwr;
	struct delayed_work power_key_emulation_work;
	const struct pm8xxx_pwrkey_emulation_platform_data *pdata;
};

static struct pmic8xxx_pwrkey_emulation *s_pwrkey;
static int pwr_on_trigger;
static u32 pwrkey_delay_ms;

static void pwrkey_sw_pressed(struct work_struct *work)
{
  if (!s_pwrkey)
		return;
	
	input_report_key(s_pwrkey->pwr, KEY_POWER, 1);
	input_sync(s_pwrkey->pwr);
	msleep(3000);
	input_report_key(s_pwrkey->pwr, KEY_POWER, 0);
	input_sync(s_pwrkey->pwr);

	return;
}

static ssize_t pwronoff_trigger_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return sprintf(buf, "%d\n", pwr_on_trigger);
}

static ssize_t pwronoff_trigger_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d\n", &pwr_on_trigger);

	if (!s_pwrkey)
		return 0;

	schedule_delayed_work(&s_pwrkey->power_key_emulation_work, round_jiffies_relative(msecs_to_jiffies(pwrkey_delay_ms)));

	return count;
}

static ssize_t pwronoff_delay_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return sprintf(buf, "%d\n", pwrkey_delay_ms);
}

static ssize_t pwronoff_delay_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d\n", &pwrkey_delay_ms);

	return count;
}

static struct kobj_attribute pwr_onoff_attr =
	__ATTR(pwr_onoff_trigger, 0444, pwronoff_trigger_show, pwronoff_trigger_store);

static struct kobj_attribute pwr_onoff_delay_attr =
	__ATTR(pwr_onoff_delay, 0444, pwronoff_delay_show, pwronoff_delay_store);

static struct attribute *g[] = {
        &pwr_onoff_attr.attr,
        &pwr_onoff_delay_attr.attr,
        NULL,
};

static struct attribute_group pwr_onoff_attr_group = {
        .attrs = g,
};

static int __devinit pmic8xxx_pwrkey_emulation_probe(struct platform_device *pdev)
{
	struct input_dev *pwr;
	int err;
	
	struct pmic8xxx_pwrkey_emulation *pwrkey;
	const struct pm8xxx_pwrkey_emulation_platform_data *pdata =
					dev_get_platdata(&pdev->dev);


	if (!pdata) {
		dev_err(&pdev->dev, "power key platform data not supplied\n");
		return -EINVAL;
	}
	
	pwrkey = kzalloc(sizeof(*pwrkey), GFP_KERNEL);
	if (!pwrkey)
		return -ENOMEM;

	pwrkey->pdata = pdata;

	pwrkey_delay_ms = pwrkey->pdata->trigger_delay_ms;

	pwr = input_allocate_device();
	if (!pwr) {
		dev_dbg(&pdev->dev, "Can't allocate power button\n");
		err = -ENOMEM;
		goto free_pwrkey;
	}

	input_set_capability(pwr, EV_KEY, KEY_POWER);

	pwr->name = "pmic8xxx_pwrkey_emulation";
	pwr->phys = "pmic8xxx_pwrkey_emulation/input0";
	pwr->dev.parent = &pdev->dev;

	err = input_register_device(pwr);
	if (err) {
		dev_dbg(&pdev->dev, "Can't register power key emulation: %d\n", err);
		goto free_input_dev;
	}

	pwrkey->pwr = pwr;

	platform_set_drvdata(pdev, pwrkey);

	s_pwrkey = pwrkey;

	INIT_DELAYED_WORK(&s_pwrkey->power_key_emulation_work, pwrkey_sw_pressed);

	err = sysfs_create_group(&pdev->dev.kobj, &pwr_onoff_attr_group);

	return 0;

free_input_dev:
	input_free_device(pwr);
free_pwrkey:
	kfree(pwrkey);
	return err;
}

static int __devexit pmic8xxx_pwrkey_emulation_remove(struct platform_device *pdev)
{
	struct pmic8xxx_pwrkey_emulation *pwrkey = platform_get_drvdata(pdev);

	sysfs_remove_group(&pdev->dev.kobj, &pwr_onoff_attr_group);
	input_unregister_device(pwrkey->pwr);
	platform_set_drvdata(pdev, NULL);
	kfree(pwrkey);

	return 0;
}

static struct platform_driver pmic8xxx_pwrkey_emulation_driver = {
	.probe		= pmic8xxx_pwrkey_emulation_probe,
	.remove		= __devexit_p(pmic8xxx_pwrkey_emulation_remove),
	.driver		= {
		.name	= PM8XXX_PWRKEY_EMULATION_DEV_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init pmic8xxx_pwrkey_emulation_init(void)
{
	return platform_driver_register(&pmic8xxx_pwrkey_emulation_driver);
}
module_init(pmic8xxx_pwrkey_emulation_init);

static void __exit pmic8xxx_pwrkey_emulation_exit(void)
{
	platform_driver_unregister(&pmic8xxx_pwrkey_emulation_driver);
}
module_exit(pmic8xxx_pwrkey_emulation_exit);

MODULE_ALIAS("platform:pmic8xxx_pwrkey_emulation");
MODULE_DESCRIPTION("PMIC8XXX Power Key emulation driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Na YoungKwan <na.youngkwan@pantech.com>");

