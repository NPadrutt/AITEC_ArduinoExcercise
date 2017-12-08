/*
 Name:      RESTClientTest.ino
 Created:   12/7/2017 12:58:45 AM
 Author:    Adrian Kauz
*/

#include <Ethernet.h>
#include <SPI.h>
#include <RestClient\RestClient.h>

RestClient client;
byte macAdrress[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ipAddress[] = { 192, 168, 1, 111 };

// the setup function runs once when you press reset or power the board
void setup() {
    client = RestClient("192.168.1.111",5000);
    Ethernet.begin(macAdrress, ipAddress);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
