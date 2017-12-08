
//zoomkat 12/26/12
//SD server test code
//open serial monitor to see what the arduino receives
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields

#include <SD.h>
#include <SPI.h>
#include <Ethernet2.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xE6, 0xAB };
byte ip[] = { 192, 168, 1, 80 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(84); //server port
String readString;

//////////////////////

void setup()
{
    Serial.begin(9600);

    // disable w5100 while setting up SD
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    Serial.print("Starting SD..");
    
    if (!SD.begin(4)) {
        Serial.println("failed");
    }
    else {
        Serial.println("ok");
    }

    Ethernet.begin(mac, ip, gateway, gateway, subnet);
    //delay(2000);
    server.begin();
    Serial.println("Ready");
}

void loop()
{
    // Create a client connection
    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();

                //read char by char HTTP request
                if (readString.length() < 100) {
                    //store characters to string 
                    readString += c;
                    Serial.print(c);
                }
                //if HTTP request has ended
                if (c == '\n') {

                    ///////////////
                    Serial.println(readString); //print to serial monitor for debuging 

                    client.println("HTTP/1.1 200 OK"); //send new page
                                                       //client.println("Content-Type: text/html");
                    client.println("Content-Type: image/jpeg");
                    //client.println("Content-Type: image/gif");
                    //client.println("Content-Type: application/x-javascript");
                    //client.println("Content-Type: text");

                    client.println();

                    //File myFile = SD.open("boom.htm");
                    File myFile = SD.open("BRIAN.JPG");
                    //File myFile = SD.open("BLUEH_SL.GIF");
                    //File myFile = SD.open("SERVOSLD.HTM");

                    if (myFile) {

                        byte clientBuf[64];
                        int clientCount = 0;

                        while (myFile.available())
                        {
                            clientBuf[clientCount] = myFile.read();
                            clientCount++;

                            if (clientCount > 63)
                            {
                                Serial.println("Packet");
                                client.write(clientBuf, 64);
                                clientCount = 0;
                            }
                        }
                        //final <64 byte cleanup packet
                        if (clientCount > 0) client.write(clientBuf, clientCount);
                        // close the file:
                        myFile.close();
                    }
                    delay(1);
                    //stopping client
                    client.stop();
                    readString = "";
                }
            }
        }
    }
}
