/*
    Ping Example
    This example sends an ICMP pings every 500 milliseconds, sends the human-readable
    result over the serial port.

    Circuit:
    * Ethernet shield attached to pins 10, 11, 12, 13
    created 30 Sep 2010
    by Blake Foster
*/

/*
    Modified by Adrian Kauz (2017.12.07)
    - Added toggling LED for visual feedback
    - Changed <Ethernet.h> to <Ethernet2.h>
    - Sending a ping every 5 sec
    - Changed destination address to "8.8.8.8" -> Google's public DNS
*/

#include <ICMPPing.h>
#include <SPI.h>
#include <Ethernet2.h>

int ledPin = 3;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB };
byte ip[] = { 192, 168, 1, 80 };
IPAddress pingAddr(8, 8, 8, 8); // Add the destination address here! For example: "8.8.8.8" for pinging Google's public DNS

SOCKET pingSocket = 0;

char buffer[256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(38400);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Begin setup... ");

    // configure LED Port
    pinMode(ledPin, OUTPUT);

    // start Ethernet
    Ethernet.begin(mac, ip);

    Serial.println("End setup... ");
}

void loop()
{
    ICMPEchoReply echoReply = ping(pingAddr, 4);

    if (echoReply.status == SUCCESS)
    {
        Serial.write("SUCCESS");
        sprintf(buffer,
            "Reply[%d] from: %d.%d.%d.%d: bytes=%d time=%ldms TTL=%d",
            echoReply.data.seq,
            echoReply.addr[0],
            echoReply.addr[1],
            echoReply.addr[2],
            echoReply.addr[3],
            REQ_DATASIZE,
            millis() - echoReply.data.time,
            echoReply.ttl);
    }
    else
    {
        Serial.write("FAIL");
        sprintf(buffer, "Echo request failed; %d", echoReply.status);
    }
    Serial.println(buffer);
    
    // toggle LED
    digitalWrite(ledPin, !digitalRead(ledPin));

    delay(5000); // Send a ping every 5s
}