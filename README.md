# China - ESP8266 D1 WiFi with MP3, RFID, and Buttons

This repository contains the code and documentation for setting up an ESP8266 D1 WiFi module with MP3, RFID, and buttons from China. The project aims to create three MP3 boxes for three children, utilizing RFID cards to select specific folders. The buttons control various functions such as play/pause, volume control, and sleep mode.

## Product Links
- ESP8266: [Link](https://a.aliexpress.com/_EQYIfMJ) - Price: €4
- RFID: [Link](https://a.aliexpress.com/_EHvVy2J) - Price: €3
- MP3: [Link](https://a.aliexpress.com/_EytHyl9) - Price: €2
- Buttons: [Link](https://a.aliexpress.com/_ExdPywr) - Price: €2

## Pin Usage on ESP8266 China Module
| Label   | GPIO  | Function                 | Safe to use? | Reason                               |
|---------|-------|--------------------------|--------------|--------------------------------------|
| D0      | GPIO16| Wake up from deep sleep  | ??           | Can I use it for deep sleep?         |
| D15/SCL | GPIO5 | RFID Connection          |              |                                      |
| D14/SDA | GPIO4 | RFID Connection          |              |                                      |
| D11 MOSI| GPIO13| MP3 Player RX            |              |                                      |
| D10 SS  | GPIO15| MP3 Player TX            |              |                                      |
| RX/DO   | GPIO3 | Button MP3 Pause         |              |                                      |
| D13/SCK | GPIO14| Button for Sleep Timer    |              |                                      |

## MP3 Connections
 MP3 - TF - 16P Pinout:
 
---| VCC   UU  BUSSY |---
---| RX         USB- |---
---| TX         USB+ |---
---| DAC_R    ADKEY1 |---
---| DAV_L    ADKEY2 |---
---| SPK +     I/O 2 |---       Button Short Press to play next / long to increase volume
---| GND         GND |---
---| SPK -     I/O 1 |---       Button Short Press to play previous / long to decrease volume

 Connection to ESP8266/Speaker
| PIN   | Speaker/ Esp8266   |
|-------|--------------------|
| SPK - | ---- Speaker Minus |
| SPK + | ---- Speaker Plus  |
| RX    | ---- GPIO13        |
| TX    | ---- GPIO15        |
| VCC   | ---- 5V            |
| GND   | ---- GND           |

## RFID Connection
RFID Pinout;

| RFID - RC522 | China ESP8266                |
|--------------|------------------------------|
|         VCC  |  ---- 3,3V                   |
|         RST  |  ---- D15/SCL/D3  ---- GPIO5 |
|         GND  |  ---- GND                    |
|         IRQ  |  ----                        |
|         MISO |  ---- D12/MISO/D6 ---- GPIO12|
|         MOSI |  ---- D11/MOSI/D7 ---- GPIO13|
|         SCK  |  ---- D13/SCK     ---- GPIO14|
|         SS   |  ---- D14/SDA/D4  ---- GPIO5 |


## Usage Instructions
- Green Button: Short press to play next / long press to increase volume.
- Yellow Button: Short press to play previous / long press to decrease volume.
- Blue Button: Activates sleep mode, turns off after 1 hour.
- Red Button: Pause/Start Button.
- White Button: Instructions on how to connect?

Note: It may be helpful to turn the China ESP8266 D1 WiFi module around and check the labels on the backside, as they may differ from the ones on the front side.
