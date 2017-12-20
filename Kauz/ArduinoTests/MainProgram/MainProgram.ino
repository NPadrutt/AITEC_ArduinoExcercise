/*
  Name:         MainProgram.ino
  Created:      12/10/2017 1:27:02 AM
  Author:       Adrian Kauz
  Commentary:   Main program for the AITEC IoT-Task.
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Include\ButtonHandler.h"
#include "Include\CameraHandler.h"
#include "Include\HttpHandler.h"
#include "Include\SeeedOLED.h"
#include "Include\Globals.h"

const int MESSAGE_MAX_LENGTH = 50;
const int WAIT_FOR_REPLY_IN_SECONDS = 60;

int secondCounter = 0;

char message[MESSAGE_MAX_LENGTH];  // 50 chars and one string-terminator

ButtonHandler button = ButtonHandler();
CameraHandler camera = CameraHandler();
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

    DEBUG_PRINT("Begin setup...");

    // Initialize Button
    button.setInputPin(2);

    // Initialize SD-Card
    if (!SD.begin(4)) {
        while (1); // If init failed, stop here
    }

    // Initialize camera
    camera.setAddress(0);
    camera.initialize();
    delay(1000);
    camera.setResolution(VGA);

    // Initialize Http-Handler
    httpHandler.setClientMACAddress(0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB);
    httpHandler.setClientIpAddress(192, 168, 1, 80);
    httpHandler.setClientUUID("18d1ff4f-91d1-4f95-988a-b278480a53ea");
    httpHandler.setServerAddress("192.168.1.200");
    httpHandler.setServerPort(80);
    httpHandler.init();

    // Initialize display
    Wire.begin();
    SeeedOled.init();                       // Initialize SEEED OLED Display
    SeeedOled.clearDisplay();               // Clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();           // Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();                // Set addressing mode to Page Mode
    SeeedOled.setTextXY(0, 0);              // Set the cursor to Xth Page, Yth Column
    DEBUG_PRINT("... setup done!");
}


/*
================
loop()
================
*/
void loop()
{
    DEBUG_PRINT("\nReady for new Event!");
    SeeedOled.clearDisplay();
    SeeedOled.putString("Ready!");

    button.waitForButtonPress();
    camera.captureImage();
    camera.saveImageToSDCard();
    delay(50);
    httpHandler.postImageToServer(camera.getCurrentImageNr());
    
    SeeedOled.clearDisplay();
    SeeedOled.putString("Wait for reply..");
    
    while (secondCounter <= WAIT_FOR_REPLY_IN_SECONDS) {
        if (httpHandler.getReplyFromServer(message, MESSAGE_MAX_LENGTH)) {
            SeeedOled.clearDisplay();
            SeeedOled.putString(message);
            break;
        }
        else {
            delay(1000);
            secondCounter++;
        }
    }

    button.waitForButtonPressed();
}