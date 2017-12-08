/*
 Name:      PushImageToServerTest.ino
 Created:   12/8/2017 1:03:14 AM
 Author:    Adrian Kauz
*/

#include <SPI.h>
#include <Ethernet2.h>
#include <SD.h>
#include <Base64.h>

byte arduinoMACAddr[]       = { 0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB }; // Arduino's MAC address
byte arduinoIPAddr[]        = { 192, 168, 1, 80 };                    // Arduino's IP Address
byte serverIPAddress[]      = { 192, 168, 1, 200 };                   // IP-Address of the webserver
int  serverPort             = 1337;                                   // Port of the webserver
unsigned long fileSize      = 0;

EthernetClient client;

void sendHttpRequest();

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

    Serial.print("- Init Ethernet... ");
    Ethernet.begin(arduinoMACAddr, arduinoIPAddr);
    Serial.println("done!");

    Serial.println("End setup...");
}


/*
================
loop()
================
*/
void loop()
{   
    sendHttpRequest();
    while (1);
}


/*
================
sendHttpRequest()
================
*/
void sendHttpRequest()
{
    Serial.println("Begin sendHttpRequest()...");

    File myFile;

    Serial.println("- Open file... ");
    if (SD.exists("BRIAN.JPG")) {
        myFile = SD.open("BRIAN.JPG");

        if (myFile) {
            Serial.println("success!");

            unsigned long contentLength = 0;

            fileSize = myFile.size();
            contentLength = (fileSize + 2 - ((fileSize + 2) % 3)) / 3 * 4; // size of the file as Base64

            if (client.connect(serverIPAddress, serverPort)) {
                Serial.println("- Connected!");
                char buffer[100];
                
                // Current content
                sprintf(buffer, "{\"sID\":%d,\"sEvent\":\"%s\"\0", 245, "Klingeln");

                // Make a HTTP request:
                client.println("POST / HTTP/1.1");
                client.println("User-Agent: Arduino");
                client.println("Host: 192.168.1.200");
                client.println("Content-Type: application/json");
                client.println("Connection: close");

                // Calculate correct length of content
                client.print("Content-Length: ");
                contentLength += strlen(buffer);
                contentLength += 11; // For the other stuff after buffer
                
                char bufferLength[6];
                itoa(contentLength, bufferLength, 10);
                client.println(bufferLength);

                client.println();
                
                // Start printing content
                client.print(buffer);
                client.print(",\"data\":\"");
                
                // Here begins the magic about converting the image to base64
                int clientCount = 0;
                char clientBuf[48]; // Must be a multiple of 3
                char clientBase64Buf[72];

                while (myFile.available()) // Is another byte to read available?
                {
                    clientBuf[clientCount] = myFile.read(); // Read one byte
                    clientCount++;

                    //
                    if (clientCount > 47)
                    {
                        base64_encode(clientBase64Buf, clientBuf, clientCount);
                        Serial.print(clientBase64Buf);
                        client.print(clientBase64Buf);
                        clientCount = 0;
                    }
                }
                
                //final <48 byte cleanup packet
                if (clientCount > 0) {
                    base64_encode(clientBase64Buf, clientBuf, clientCount);
                    Serial.print(clientBase64Buf);
                    client.print(clientBase64Buf);
                }
                // close the file:
                myFile.close();
                
                client.println("\"}");
                // End printing content

                client.stop();
                Serial.println("- Disconnected!");
            }
            else {
                Serial.println("- Connection failed!");
            }
        }
        else {
            Serial.println("failed!");
        }
    }
    else {
        Serial.println("file not found!");
    }

    Serial.println("End sendHttpRequest()...");
}
