# Room LED control system WIP
Simple, but complex system acting as base for creating smart room.

##### PC app
.NET C# application, performs FFT of speakers audio signal, adjusts it with user preferences and converts to HSV color data. Then sends it as UDP packets via LAN to Raspberry Pi.

Code coming soon.

##### Raspberry Pi
The raspberry part are two servers:
- Apple Homekit server, based on HAP-NodeJS
- Node.JS simple UDP receiver

The first one allows me to control color of RGB LEDs from my iPhone or iPad. It imitates my LEDs as Homekit accessories, and it's a bridge between my iPhone and XMega LED driver. It receives commands over WiFi from iPhone and executes python script to send them through UART to XMega microcontroller.
Also, Raspberry has a DS18B20 temperature sensor connected to its 1Wire port. This sensor is also visible as Homekit accessory.

The second one is very simple Node app (`udp-rgb-server.js`). It acts as a bridge between my PC and XMega. It just receives UDP color data from PC, converts it to proper format and sends to XMega using UART.

#### XMega256A3BU
For now, it acts only as PWM driver, controlled from UART. It converts UART color messages into PWM values. It has 8 PWM channels: 2xRGB + 2 monochrome. One is yet unused.

There is an Atmel Studio 7 solution in `/XMega` directory.

This project will be continued, but I have no time for that now.