# Project VC 5

Note: This project is still under development.

finished parts:
- Firmware
- PC app (basic functionality)
- schematic

under development:
- 3D model for the case
- PC app (features for better useability)

future:
- support for Linux


## General description
The aim of this project is to create an external device, which allows the user to adjust the volume of various programs on a host PC. 

## Hardware
A number of rotaries are available as input (number set to 5 within the config file of this project) and LED's and displays to show any kind of status.
The schematic for the device can be found at _Harware/schematic_.

## Software
The software consists of two parts.

### 1. Firmware
The firmware is written for the Raspberry Pi RP2040. It communicates via USB with the host PC. The firmware only transmits data from the rotaries to the host PC and executes commands from the host PC. These commands can for example set the color of the LED and change the content shown on the displays.


### 2. PC application
The PC application (only Windows) acts as the executing intelligence. It takes the data from the rotaries and processes it. The volume of five different audio channels is controlled. The application can also send commands to the device to e.g. set the LED color or write information on the displays.
