/*
  Name:         MainProgram.ino
  Created:      12/10/2017 1:27:02 AM
  Author:       Adrian Kauz
  Commentary:   Main program for the AITEC IoT-Task.
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "includes\ButtonPress.h"
#include "includes\Camera.h"
#include "includes\HttpHandler.h"
#include "includes\SeeedOLED.h"

const int buttonPin = 2;

Camera camera = Camera();
HttpHandler httpHandler = HttpHandler();

/*
================
setup()
================
*/
void setup()
{   
    // Initialize Serial
    Serial.begin(115200);
    
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }

    Serial.println(F("Start setup..."));

    // Initialize SD-Card
    if (!SD.begin(4)) {
        while (1); // If init failed, stop here
    }

    // Initialize button
    prepareButton(buttonPin);    

    // Initialize camera
    camera.setAddress(0);
    camera.initialize();
    camera.setResolution(VGA);

    // Initialize Http-Handler
    httpHandler.setClientMACAddress(0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB);
    httpHandler.setClientIpAddress(192, 168, 1, 80);
    httpHandler.setClientId("18d1ff4f-91d1-4f95-988a-b278480a53ea");
    httpHandler.setServerIpAddress(192, 168, 1, 200);
    httpHandler.setServerPort(1337);
    httpHandler.init();

    // Initialize display
    Wire.begin();
    SeeedOled.init();                       // Initialize SEEED OLED Display
    SeeedOled.clearDisplay();               // Clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();           // Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();                // Set addressing mode to Page Mode
    SeeedOled.setTextXY(0, 0);              // Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Hello World!");    // Print the String

    Serial.println(F("End setup..."));
}


/*
================
loop()
================
*/
void loop()
{
    waitForButtonPress(buttonPin);
    camera.captureImage();
    camera.saveImageToSDCard();
    
    httpHandler.postImageToServer(camera.getCurrentImageNr());
    httpHandler.postImageToServer(0);
    while (1);

    /*if (waitForUserReply()) {
        displayMessage();
    }*/
}