/*
Name:         HttpHandler.cpp
Created:      12/12/2017 2:22:02 AM
Author:       Adrian Kauz
Commentary:   HTTPHandler for PUSH- and GET-Requests.
*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include "Base64.h"
#include "HttpHandler.h"
#include "Globals.h"

EthernetClient client;

/*
================
setMACAddress()
================
*/
void HttpHandler::setClientMACAddress(byte firstByte, byte secondByte, byte thirdByte, byte fourthByte, byte fifthByte, byte sixthByte)
{
    arduinoMACAddr[0] = firstByte;
    arduinoMACAddr[1] = secondByte;
    arduinoMACAddr[2] = thirdByte;
    arduinoMACAddr[3] = fourthByte;
    arduinoMACAddr[4] = fifthByte;
    arduinoMACAddr[5] = sixthByte;
}


/*
================
setIpAddress()
================
*/
void HttpHandler::setClientIpAddress(byte firstByte, byte secondByte, byte thirdByte, byte fourthByte)
{
    arduinoIpAddr[0] = firstByte;
    arduinoIpAddr[1] = secondByte;
    arduinoIpAddr[2] = thirdByte;
    arduinoIpAddr[3] = fourthByte;
}


/*
================
setClientId()
================
*/
void HttpHandler::setClientId(char newClientId[])
{
    sprintf(clientID, "%s", newClientId);
}


/*
================
setServerAddress()
================
*/
void HttpHandler::setServerAddress(char* newServerAddress)
{
    serverAddress = newServerAddress;
}


/*
================
setServerPort()
================
*/
void HttpHandler::setServerPort(int newServerPortNr)
{
    serverPortNr = newServerPortNr;
}


/*
================
init()
================
*/
void HttpHandler::init()
{
    Ethernet.begin(arduinoMACAddr, arduinoIpAddr);
}


/*
================
postImageToServer()
================
*/
void HttpHandler::postImageToServer(int currImageNumber)
{
    DEBUG_PRINT(" - Enter [HttpHandler::postImageToServer()]");

    File myFile;

    // Adjust "IMAGE000.JPG" to current image number
    fileName[5] = currImageNumber / 100 + '0';
    fileName[6] = currImageNumber / 10 + '0';
    fileName[7] = currImageNumber % 10 + '0';
    
    if (SD.exists(fileName)) {
        myFile = SD.open(fileName);

        if (myFile) {
            unsigned long contentLength = 0;
            unsigned long fileSize = 0;

            fileSize = myFile.size();
            contentLength = (fileSize + 2 - ((fileSize + 2) % 3)) / 3 * 4; // size of the file as Base64

            if (client.connect(serverAddress, serverPortNr)) {
                char buffer[50];

                // Variable content
                sprintf(buffer, "{\"clientID\":\"%s\"\0", clientID);

                // Make a HTTP request:
                client.println(F("POST /iot/test.php HTTP/1.1"));
                client.println(F("User-Agent: Arduino"));
                client.println(F("Host: 192.168.1.200"));
                client.println(F("Content-Type: application/json"));
                client.println(F("Connection: close"));

                // Calculate correct length of content
                client.print(F("Content-Length: "));
                contentLength += strlen(buffer) + 41; // Length of variable content and (1) and (2)

                char bufferLength[8];
                sprintf(bufferLength, "%lu", contentLength);

                client.println(bufferLength);
                client.println();

                // Start printing content
                client.print(buffer);
                client.print(F(",\"base64Image\":\"data:image/jpeg;base64,")); // (1)

                // Here begins the magic about converting the image to base64 and sending it via Ethernet
                int clientCount = 0;
                char clientBuf[FILE_BUFFER_SIZE]; // Must be a multiple of 3
                char clientBase64Buf[FILE_BASE64_BUFFER_SIZE];

                DEBUG_PRINT("   - Uploading... ");
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

                DEBUG_PRINT("   - ...done!");

                // close the file:
                myFile.close();

                client.println("\"}"); // (2)
                // End printing content

                client.stop();
            }
            else {
                DEBUG_PRINT("   - Unable to connect to the server!");
            }
        }
        else {
            DEBUG_PRINT("   - Unable to open file!");
        }
    }
    else {
        DEBUG_PRINT("   - File doesn't exist!");
    }

    DEBUG_PRINT(" - Leave [HttpHandler::postImageToServer()]");
}