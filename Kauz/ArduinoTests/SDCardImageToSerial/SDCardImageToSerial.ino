/*
Name:      SDCardImageToSerial.ino
Created:   12/8/2017 1:03:14 AM
Author:    Adrian Kauz
Commentary: Read a file from a SD-Card, convert it to Base64 and send it directly via serial-port to destination
*/

#include <SPI.h>
#include <SD.h>
#include <Base64.h>

unsigned long fileSize = 0;
void sendImageToSerial();

/*
================
setup()
================
*/
void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(38400);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }

    Serial.println("Begin setup...");

    Serial.print("- Init SD... ");
    if (!SD.begin(4)) {
        Serial.println("failed!");
        while (1);
    }
    else {
        Serial.println("success!");
    }

    Serial.println("End setup...");
}


/*
================
loop()
================
*/
void loop()
{
    sendImageToSerial();
    while (1);
}


/*
================
sendImageToSerial()
================
*/
void sendImageToSerial()
{
    Serial.println("Begin sendImageToSerial()...");

    File myFile;

    Serial.println("- Open file... ");
    if (SD.exists("PIC00.JPG")) {
        myFile = SD.open("PIC00.JPG");
        if (myFile) {
            Serial.println("success!");

            unsigned long contentLength = 0;

            fileSize = myFile.size();
            contentLength = (fileSize + 2 - ((fileSize + 2) % 3)) / 3 * 4; // size of the file as Base64

            Serial.print("Base64 content length: ");
            Serial.println(contentLength);

            // Here begins the magic about converting the image to base64
            int clientCount = 0;
            char clientBuf[48]; // Must be a multiple of 3
            char clientBase64Buf[72];

            Serial.println("Start converting...\n");
            while (myFile.available()) // Is another byte to read available?
            {
                clientBuf[clientCount] = myFile.read(); // Read one byte
                clientCount++;

                //
                if (clientCount > 47)
                {
                    base64_encode(clientBase64Buf, clientBuf, clientCount);
                    Serial.print(clientBase64Buf);
                    clientCount = 0;
                }
            }

            //final <48 byte cleanup packet
            if (clientCount > 0) {
                base64_encode(clientBase64Buf, clientBuf, clientCount);
                Serial.print(clientBase64Buf);
            }
            Serial.println("\n... fertig.\n");
            // close the file:
            myFile.close();
        }
        else {
            Serial.println("failed!");
        }
    }
    else {
        Serial.println("file not found!");
    }

    Serial.println("End sendImageToSerial()...");
}
