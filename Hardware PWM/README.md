# Hardware PWM

Increases the brightness of a led on the launchpad when a button is pressed. The other led lights up while the button is being pressed.
This uses timers to directly control the pins to generate PWM.

The MSP430G2553 and MSP430F5529 are officially supported, and the source code is in the g2553 and f5529 folders, respectivly.

# Usage

By default, this uses P1.0 for th button led, P1.6 for the brightness led, and P1.3 for the button on the g2553.
The f5529 uses P4.7 for the button led, P1.2 for the brightness led, and P1.1 for the button.
Note that the f5529 launchpa does not have a led on p1.2; a jumper can be run to the P1.0 led.

# Compiling and uploading

Ensure that you have msp430gcc installed. In particular, `msp430-elf-gcc` and `msp430-elf-objcopy` are used to compile the program and convert the elf to a hex for uploading.

To flash the msp430, you will need the official TI MSP Flasher installed. It will need to be avaiable as `mspflash`. This allows a wrapper script to set the `LD_LIBRARY_PATH` correctly for MSP Flasher.

Simply type `make` in the correct directory for your processor, and the program will be compiled and uploaded to an attached Launchpad.

