# i.MX233
- I2C port

Using i.MX233 board (ex. OLinuXino from olimex) is a great experience for peoples who are new to embedded system. However, after searching google for finding a suitable linux image you will notify that I2C port is not available.

Command "i2cdetect -l" results nothing, because I2C driver libraries are not added to the kernel. So, the best solution is to create a new image with your specific requirements. Dealing with YOCTO or OE-CORE and finding correct BSP files needs experience.

Here we have presented Software I2C compatible with i.MX233 board. Just you need to compile the code using GCC and make use of it.

Not to mentioned that even here I2C will not be detectable by "/dev/i2c-0" or "/dev/i2c-1", because we have implemented a soft I2C protocol which simulate the behaviour of actual I2C but by manually turn on/off the pins.

In order to change the I2C pins, please open-up "SoftI2CMaster.c" and change sda_pin/ scl_pin to your selected ones. The pin numbers are the actual i.MX233 GPIO pin names, and pin_bank should set to 0.



Compile code for gcc (Please cd to the target directory then use the following command):

gcc *.c -Wall -o MY_I2C
