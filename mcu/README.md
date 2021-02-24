# MCU

The code in this folder is for the main MCU in the NUB. We are using the Atmel ATmega328PB MCU to handle relaying messages between other NUBs and connected Android devices.

## How to Build

This code is developed using Microchip MPLabX with the XC8 compiler. This can be installed on Windows or Linux. If you're using Arch Linux:

``` bash
$ paru -S --needed microchip-mplabx-bin microchip-mplabxc8-bin # Assuming you use paru as an AUR helper
```

You then should be able to just open up this project and compile.
