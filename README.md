# nub-firmware

Firmware for NUB(Nodal Hub), a long range mesh messaging system. Capstone Project for Electrical Engineering at the University of Alberta.

## Frame Format

### NUB to NUB

| TAG | Message Type | Size of Path | Path | Data Size | Data |
| --- | ------------ | ------------ | ---- | --------- | ---- |
| ```0x02``` | 1 Byte | 1 Byte | Variable | 2 Bytes | ```Data Size``` Bytes |

### Passing through ATmega

| TAG | Data Len | Message Type | Data Size | Data |
| --- | ------------ | ------------ | ---- | --------- |
| ```0x02``` | 1 Byte | 1 Byte | 2 Bytes | ```Data Size``` bytes |

### Message Types

| Message Type Byte | Message |
| ------- | ----------------- |
| 0x01 | Text Messages |
| 0x02 | Connected/Disconnected phone |
| 0x03 | Adjust NUB Mesh |
| 0x04 | Message Acknowledgement |

## To-Do

- [x] Access point (mostly done)
- [x] Communications with Android app
- [x] UART communication between MCU and ESP8266
- [x] UART communication between MCU and transceiver
- [ ] ~~Power management (Sleep, power button)~~ batteries not allowed for project anymore
- [ ] Status LEDs
- [ ] Serial output? (For debugging)
