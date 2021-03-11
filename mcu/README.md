# MCU

The code in this folder is for the main MCU in the NUB. We are using the Atmel ATmega328PB MCU to handle relaying messages between other NUBs and connected Android devices.

## How to Build

This code is developed using Microchip MPLabX with the XC8 compiler. This can be installed on Windows or Linux. If you're using Arch Linux:

``` bash
$ paru -S --needed microchip-mplabx-bin microchip-mplabxc8-bin # Assuming you use paru as an AUR helper
```

You then should be able to just open up this project and compile.

## Design Goals

There are several design goals we want to meet for the best performance:

- [ ] Transfer data without variables when possible. We want to ensure there is enough memory for buffers.
- [x] We will have one buffer for each direction of transmission for each connected device.
- [ ] Achieve a 19200 or 115200 baud rate with the long-range transceiver.
- [ ] Achieve a 115200 baud rate with the ESP8266.
- [ ] Protocol to decide when to process what data.
- [ ] Choose a CPU frequency that works at 3.3V, while also achieving the baud rates listed above.
- [ ] Buffer sizes that fit in memory, 256 or 512 bytes should be more than enough.
