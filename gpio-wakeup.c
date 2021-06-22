/*
 * GPIO wakeup driver for ODROID N2
 *
 * Copyright (C) 2021 Andrew Ivanchuk
 *
 * This file was based on: drivers/amlogic/input/keyboard/gpio_keypad.c
 * 	Copyright (C) 2017 Amlogic, Inc. All rights reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

#define DRV_NAME        "gpio-wakeup"

/*The value is taken from drivers/amlogic/input/keyboard/gpio_keypad.c of 4.9.x Amlogic kernel*/
#define AO_DEBUG_REG0           ((0x28 << 2))

static unsigned long gpiopower=479;

static int gpio_wakeup_probe(struct platform_device *pdev)
{
        void __iomem *ao_reg=NULL;
	struct resource *res;
	int val;

	dev_info(&pdev->dev,"test driver initializing...\n");
	
	/* Values are taken from gpio_keypad section of meson64_odroidn2.dts 
	 * file from Hardkernel's 4.9.x kernel: 0xFF800000,0x400 */
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(res)	
		ao_reg=ioremap(res->start,res->end - res->start);
	else
		dev_err(&pdev->dev,"cannot obtain i/o mem region\n");
	
	if (ao_reg) {
		val = readl((ao_reg + AO_DEBUG_REG0));
		val |= (gpiopower << 16);
		writel(val, (ao_reg + AO_DEBUG_REG0));
		dev_info(&pdev->dev,"successfully set up power button #%ld for wakeup at 0x%lx\n",
				gpiopower,(unsigned long) res->start+AO_DEBUG_REG0);
		iounmap(ao_reg);
	}

	return 0;
}
static int gpio_wakeup_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "test driver unloaded\n");
	return 0;
}

static const struct of_device_id gpio_wakeup_of_match[] = {
        { .compatible = "amlogic,gpio-wakeup", },
        { },
};
MODULE_DEVICE_TABLE(of, gpio_wakeup_of_match);

static struct platform_driver gpio_wakeup_driver = {
        .probe  = gpio_wakeup_probe,
	.remove = gpio_wakeup_remove,
        .driver = {
                .name   = DRV_NAME,
                .of_match_table = gpio_wakeup_of_match,
        },
};

module_platform_driver(gpio_wakeup_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO wakeup driver for ODROID N2");
MODULE_ALIAS("platform:" DRV_NAME);
MODULE_INFO(intree, "Y");


