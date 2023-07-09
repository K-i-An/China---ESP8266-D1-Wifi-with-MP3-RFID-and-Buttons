# China---ESP8266-D1-Wifi-with-MP3-RFID-and-Buttons

Links to the products:
4€ ~ ESP8266 https://a.aliexpress.com/_EQYIfMJ
3€ ~ RFID https://a.aliexpress.com/_EHvVy2J
2€ ~ MP3 https://a.aliexpress.com/_EytHyl9
2€ ~ Buttons https://a.aliexpress.com/_ExdPywr

#PIN usage on ESP8266 China Module:
#Label  	  GPIO  	  What I use it for   Safe to use?  	Reason
#D0	      GPIO16		                    HIGH at boot, used to wake up from deep sleep ?? Can I use it for deepsleep?
#D15/SCL   GPIO5		  RFID Connection
#D14/SDA	  GPIO4		  RFID Connection
#D11 MOSI  GPIO13		MP3 Player RX
#D10 SS    GPIO15		MP3 Player TX
#RX/DO     GPIO3		  Button MP3 Pause
#D13/SCK   GPI14		  Button for Sleep Timer


MP3 - TF - 16P
---| VCC   UU  BUSSY |---
---| RX         USB- |---
---| TX         USB+ |---
---| DAC_R    ADKEY1 |---
---| DAV_L    ADKEY2 |---
---| SPK +     I/O 2 |---       Button Short Press to play next / long to increase volume
---| GND         GND |---
---| SPK -     I/O 1 |---       Button Short Press to play previous / long to decrease volume

SPK - ---- Speaker Minus
SPK + ---- Speaker Plus
RX    ---- GPIO13
TX    ---- GPIO15
VCC   ---- 5V
GND   ---- GND


RFID - RC522 | China ESP8266
        VCC  |  ---- 3,3V
        RST  |  ---- D15/SCL/D3  ---- GPIO5
        GND  |  ---- GND
        IRQ  |  ---- 
        MISO |  ---- D12/MISO/D6 ---- GPIO12
        MOSI |  ---- D11/MOSI/D7 ---- GPIO13
        SCK  |  ---- D13/SCK     ---- GPIO14
        SS   |  ---- D14/SDA/D4  ---- GPIO5

It helped to turn the China ESP8266 D1 Wifi around and see the labels on the backside of the chip. They differ from the ones on the front side.

Now I hope to establish 3x MP3 Box for my three childreen.
The RFID Cards are for reading a specific folder. We want to tune them up later. To show which MP3 they are playing.
Button Green : Button Short Press to play next / long to increase volume
Button Yellow: Button Short Press to play previous / long to decrease volume
Button Blue  : Sleep Modus will turn off in 1 hour
Button Red   : Pause/Start Button
Button White : how to connect?
