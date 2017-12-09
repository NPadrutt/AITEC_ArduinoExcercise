/*
 Name:      PushImageToServerTest.ino
 Created:   12/8/2017 1:03:14 AM
 Author:    Adrian Kauz
*/

#include <SPI.h>
#include <Ethernet2.h>
#include <SD.h>
#include <Base64.h>

#define FILE_BUFFER_SIZE 48
#define FILE_BASE64_BUFFER_SIZE 72

byte arduinoMACAddr[]       = { 0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB }; // Arduino's MAC address
byte arduinoIPAddr[]        = { 192, 168, 1, 80 };                    // Arduino's IP Address
byte serverIPAddress[]      = { 192, 168, 1, 200 };                   // IP-Address of the webserver
int  serverPort             = 1337;                                   // Port of the webserver

EthernetClient client;

void postHttpRequest();

/*
================
setup()
================
*/
void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(38400);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }

    // Init SD-Card
    if (!SD.begin(4)) {
        while (1); // If init failed, stop here
    }

    Ethernet.begin(arduinoMACAddr, arduinoIPAddr);
}


/*
================
loop()
================
*/
void loop()
{   
    postHttpRequest();
    while (1);
}


/*
================
postHttpRequest()
================
*/
void postHttpRequest()
{
    File myFile;

    if (SD.exists("BRIAN.JPG")) {
        myFile = SD.open("BRIAN.JPG");

        if (myFile) {
            unsigned long contentLength = 0;
            unsigned long fileSize = 0;

            fileSize = myFile.size();
            contentLength = (fileSize + 2 - ((fileSize + 2) % 3)) / 3 * 4; // size of the file as Base64

            if (client.connect(serverIPAddress, serverPort)) {
                char buffer[50];
                
                // Variable content
                sprintf(buffer, "{\"sID\":%d,\"sEvent\":\"%s\"\0", 245, "Klingeln");

                // Make a HTTP request:
                client.println(F("POST / HTTP/1.1"));
                client.println(F("User-Agent: Arduino"));
                client.println(F("Host: 192.168.1.200"));
                client.println(F("Content-Type: application/json"));
                client.println(F("Connection: close"));

                // Calculate correct length of content
                client.print(F("Content-Length: "));
                contentLength += strlen(buffer) + 11;
                
                char bufferLength[6];
                itoa(contentLength, bufferLength, 10);
                client.println(bufferLength);
                
                client.println();
                
                // Start printing content
                client.print(buffer);
                client.print(F(",\"data\":\""));
                
                // Here begins the magic about converting the image to base64 and sending it via Ethernet
                int clientCount = 0;
                char clientBuf[FILE_BUFFER_SIZE]; // Must be a multiple of 3
                char clientBase64Buf[FILE_BASE64_BUFFER_SIZE];

                while (myFile.available()) // Is another byte to read available?
                {
                    clientBuf[clientCount] = myFile.read(); // Read one byte
                    clientCount++;

                    if (clientCount > FILE_BUFFER_SIZE - 1)
                    {
                        base64_encode(clientBase64Buf, clientBuf, clientCount);
                        //Serial.print(clientBase64Buf); // For serial output to check if encoding was successful
                        client.print(clientBase64Buf);
                        clientCount = 0;
                    }
                }
                
                //final <48 byte cleanup packet
                if (clientCount > 0) {
                    base64_encode(clientBase64Buf, clientBuf, clientCount);
                    //Serial.print(clientBase64Buf); // For serial output to check if encoding was successful
                    client.print(clientBase64Buf);
                }
                // close the file:
                myFile.close();
                
                client.println("\"}");
                // End printing content

                client.stop();
            }
        }
    }
}
