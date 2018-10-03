# Debounce

Toggles the LED on the launchpad when the button is pressed. Uses software debouncing to ensure there are no mispresses.

The MSP430G2553 and MSP430FR2311 are officially supported, and the source code is in the g2553 and f5529 folders, respectivly.

# Usage

By default, this uses P1.0 for the led and P1.1 for the button for the fr2311 and P1.6 for the led and P1.2 for the button on the g2553. This can be changed by changing the #defines at the beginning of main.c

# Compiling and uploading

Ensure that you have msp430gcc installed. In particular, `msp430-elf-gcc` and `msp430-elf-objcopy` are used to compile the program and convert the elf to a hex for uploading.

To flash the msp430, you will need the official TI MSP Flasher installed. It will need to be avaiable as `mspflash`. This allows a wrapper script to set the `LD_LIBRARY_PATH` correctly for MSP Flasher.

Simply type `make` in the correct directory for your processor, and the program will be compiled and uploaded to an attached Launchpad.

