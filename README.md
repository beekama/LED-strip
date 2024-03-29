# LED-strip # 
Webinterface and arduino-code for the *WS2812B-LED-Strip* with 300 leds.
For the implementation I used:
* *Raspberry-Pi 3* for the Webinterface
* *Espressif ESP32-WROOM-32* from *Espressif* for direct communication with the LED-strip


## Webinterface ##
The webinterface is written in Python using Flask and Bootstrap.
Data input is parsed and send to the ESP32 as simple HTTP-message.


## Arduino Code ##
The ESP32 also requires connection to the local network to receive the messages from the Raspberry-Pi.
Additionally it needs to be physically connected to the LED-strip in order to transfer the data to the leds.<br><br>
With **OTA**(over the air)-update we no longer have to rely on a cable to load a new sketch onto the ESP.<br>
However there are two things to keep in mind:
* the first OTA-code has to be transferred via cable
* OTA requires a firewall-rule for espota.exe on Windows 

## Requirements ##
**jscolor**¹ for the color-pickers <br>
**Bootstrap**² <br> <br>
1: https://jscolor.com/download/ <br>
2: https://getbootstrap.com/docs/5.0/getting-started/download/
