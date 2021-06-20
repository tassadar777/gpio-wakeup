/* 
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

MODULE_LICENSE("GPL");
MODULE_INFO(intree, "Y");

/*The value is taken from drivers/amlogic/input/keyboard/gpio_keypad.c of 4.9.x Amlogic kernel*/
#define AO_DEBUG_REG0           ((0x28 << 2))

static unsigned long gpiopower=479;

static int gpio_wakeup_init(void)
{
        void __iomem *ao_reg=NULL;
	/*struct resource *res;*/
	int val;

	printk(KERN_INFO "[gpio-wakeup] test driver initializing...\n");
	
	/* Values are taken from gpio_keypad section of meson64_odroidn2.dts 
	 * file from Hardkernel's 4.9.x kernel: */

	/* Does not work for now
	res=request_mem_region(0xFF800000,0x400,"gpio-wakeup");
	if(res)	
		ao_reg=ioremap(res->start,res->end - res->start);
	else
		printk(KERN_ERR "[gpio-wakeup] cannot obtain i/o mem region\n");
	*/
	
	ao_reg=ioremap(0xFF800000,0x400);

	if (ao_reg) {
		val = readl((ao_reg + AO_DEBUG_REG0));
		val |= (gpiopower << 16);
		writel(val, (ao_reg + AO_DEBUG_REG0));
		printk(KERN_INFO "[gpio-wakeup] successfully set up power button #%ld for wakeup at 0x%x\n",
				gpiopower,0xFF800000+AO_DEBUG_REG0);
		iounmap(ao_reg);
	}

	return 0;
}
static void gpio_wakeup_exit(void)
{
	printk(KERN_INFO "[gpio-wakeup] test driver unloaded\n");
}

module_init(gpio_wakeup_init);
module_exit(gpio_wakeup_exit);

