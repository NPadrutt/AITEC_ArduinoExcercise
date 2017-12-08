# HSLU HS17 AITEC - IoT Task - Kauz Stuff
Here you can find all my code for the Advanced Internet Technologies (AITEC) Arduino "IoT" Task.<br>


  
## Getting started
You can simple copy the code from the *.ino-Files into your Arduino-IDE or download the whole thing and open the solution "ArduinoTests.sln" with Visual Studio 2017.  
Make sure you have installed Visual Studio with C++ support and the Arduino IDE-Plugin by Visual Micro. You can download the plugin here: http://www.visualmicro.com/  
<br>
Folder includes following codes for testing and evaluation:  
- [x] [**Fading LED**](ArduinoTests/FadingLED/FadingLED.ino) _(2017.12.05)_
- [x] [**SDCardInitTest**](ArduinoTests/SDCardInitTest/SDCardInitTest.ino) _(2017.12.06)_
- [x] [**SDCardReadWriteTest**](ArduinoTests/SDCardReadWriteTest/SDCardReadWriteTest.ino) _(2017.12.06)_
- [x] [**PingTest (with modified library)**](ArduinoTests/PingTest/PingTest.ino) _(2017.12.07)_
- [x] [**WebClientTest**](ArduinoTests/WebClientTest/WebClientTest.ino) _(2017.12.07)_
- [x] [**PushTest**](ArduinoTests/PushTest/PushTest.ino) _(2017.12.08)_
- [x] [**WebServerTest**](ArduinoTests/WebServerTest/WebServerTest.ino) _(2017.12.08)_
- [x] [**PushImageToServerTest**](ArduinoTests/PushImageToServerTest/PushImageToServerTest.ino) _(2017.12.08)_
- [x] [**SDCardImageToSerial**](ArduinoTests/SDCardImageToSerial/SDCardImageToSerial.ino) _(2017.12.08)_
- [ ] [**RestClientTest**](ArduinoTests/RestClientTest/RestClientTest.ino)

_(Checkbox is marked if test was successful)_<br><br>


## Notes
### Ethernet-Module
There are two versions of the Ethernet-Shield. One with the W5100 chipset and another with the W5500 chipset.<br>
You **can't** use the same "Ethernet"-Library for both versions!  
- With Arduino Ethernet Shield (Wiznet W5100 chipset):<br>
Use `#include <Ethernet.h>`
- With Arduino Ethernet Shield 2 (Wiznet W5500 chipset):<br>
Use `#include <Ethernet2.h>`

Maybe you have to add the Ethernet2-Library manually to your project. Get it from [**here**](https://github.com/adafruit/Ethernet2).