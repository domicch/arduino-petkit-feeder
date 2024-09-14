# Arduino for Petkit Feeder

## What is this about?
The PCB board of my Petkit Smart Feeder P512 board was burnt due to wrong transformer being plugged in accidentally. To fix that, the board is replaced by an Arduino UNO board and most of the hardware of the machine are reused and wired to the board. Additional hardware added include:
- Speed measuring IR infrared slotted optical optocoupler module photo interruptor sensor
- DS3231 real time clock
- L298N motor driver
- PCF8574 LCD display with I2C interface

## How to run?
Prerequisite for connector to the board with Arduino IDE on Ubuntu
```shell
# if the target port doesn't show in the IDE
sudo apt remove brltty
# if IDE says permission error connecting to /dev/ttyUSB0
sudo usermod -a -G dialout dominic
```
