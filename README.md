gpio-wakeup

This is an experimental driver to enable a wakeup button on ODROID N2 SBC running a 5.x kernel.
For now the pin value is hardcoded into the source file (gpiopower=479, which corresponds to pin 11). 
To set it to a different value, find your signal pin export number from this table: https://wiki.odroid.com/odroid-n2/hardware/expansion_connectors.

To build the kernel module:
 $ make

To test the wakeup button:
 $ sudo insmod gpio-wakeup.ko
 $ poweroff

and after the computer shutdown long press the power button.
