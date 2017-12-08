#include <SPI.h>
#include <arduino.h>
#include <SD.h>

#define PIC_PKT_LEN    128                  //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     Serial
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIC_FMT        PIC_FMT_VGA

File myFile;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const byte cameraAddr = (CAM_ADDR << 5);  // addr
const int buttonPin = 2;                 // the number of the pushbutton pin
unsigned long picTotalLen = 0;            // picture length
int picNameNum = 0;

/*********************************************************************/
void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
    printText("Init..");
    Serial.begin(115200);
    pinMode(buttonPin, INPUT);    // initialize the pushbutton pin as an input
    Serial.println("Initializing SD card....");
    
    if (!SD.begin(4)) 
    {
        Serial.print("sd init failed");
        return;
    }
    Serial.println("sd init done.");
    printText("Init.. Done");
    initialize();
}
/*********************************************************************/
void loop()
{
    printText("Hallo :)");
  
    int n = 0;
    while(1){
        Serial.println("\r\nPress the button to take a picture");
        while (digitalRead(buttonPin) == LOW);      //wait for buttonPin status to HIGH
        if(digitalRead(buttonPin) == HIGH){
            delay(20);                               //Debounce
            if (digitalRead(buttonPin) == HIGH)
            {
                printText("Bitte lächeln..");
                Serial.println("\r\nbegin to take picture");
                delay(200);
                if (n == 0) preCapture();
                Capture();
                GetData();
            }
            printText("Bild wurde gemacht und wird gesendet.");
            Serial.print("\r\nTaking pictures success ,number : ");
            Serial.println(n);
            n++ ;

            // Bild senden

            printText("Hallo :)");
        }
    }
}

void printText(char text[])
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(text);  
    display.display();
}

/*********************************************************************/
void clearRxBuf()
{
    while (Serial.available())
    {
        Serial.read();
    }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len)
{
    for (char i = 0; i < cmd_len; i++) Serial.print(cmd[i]);
}
/*********************************************************************/
void initialize()
{
    char cmd[] = {0xaa,0x0d|cameraAddr,0x00,0x00,0x00,0x00} ;
    unsigned char resp[6];

    Serial.setTimeout(500);
    while (1)
    {
        //clearRxBuf();
        sendCmd(cmd,6);
        if (Serial.readBytes((char *)resp, 6) != 6)
        {
            continue;
        }
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0)
        {
            if (Serial.readBytes((char *)resp, 6) != 6) continue;
            if (resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break;
        }
    }
    cmd[1] = 0x0e | cameraAddr;
    cmd[2] = 0x0d;
    sendCmd(cmd, 6);
    Serial.println("\nCamera initialization done.");
}
/*********************************************************************/
void preCapture()
{
    char cmd[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT };
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
    }
}
void Capture()
{
    char cmd[] = { 0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff ,0};
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) break;
    }
    cmd[1] = 0x05 | cameraAddr;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = 0;
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) break;
    }
    cmd[1] = 0x04 | cameraAddr;
    cmd[2] = 0x1;
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0)
        {
            Serial.setTimeout(1000);
            if (Serial.readBytes((char *)resp, 6) != 6)
            {
                continue;
            }
            if (resp[0] == 0xaa && resp[1] == (0x0a | cameraAddr) && resp[2] == 0x01)
            {
                picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
                Serial.print("picTotalLen:");
                Serial.println(picTotalLen);
                break;
            }
        }
    }

}
/*********************************************************************/
void GetData()
{
    unsigned int pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
    if ((picTotalLen % (PIC_PKT_LEN-6)) != 0) pktCnt += 1;

    char cmd[] = { 0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00 };
    unsigned char pkt[PIC_PKT_LEN];

    char picName[] = "pic00.jpg";
    picName[3] = picNameNum/10 + '0';
    picName[4] = picNameNum%10 + '0';

    if (SD.exists(picName))
    {
        SD.remove(picName);
    }

    myFile = SD.open(picName, FILE_WRITE);
    if(!myFile){
        Serial.println("myFile open fail...");
    }
    else{
        Serial.setTimeout(1000);
        for (unsigned int i = 0; i < pktCnt; i++)
        {
            cmd[4] = i & 0xff;
            cmd[5] = (i >> 8) & 0xff;

            int retry_cnt = 0;
            retry:
            delay(10);
            clearRxBuf();
            sendCmd(cmd, 6);
            uint16_t cnt = Serial.readBytes((char *)pkt, PIC_PKT_LEN);

            unsigned char sum = 0;
            for (int y = 0; y < cnt - 2; y++)
            {
                sum += pkt[y];
            }
            if (sum != pkt[cnt-2])
            {
                if (++retry_cnt < 100) goto retry;
                else break;
            }

            myFile.write((const uint8_t *)&pkt[4], cnt-6);
            //if (cnt != PIC_PKT_LEN) break;
        }
        cmd[4] = 0xf0;
        cmd[5] = 0xf0;
        sendCmd(cmd, 6);
    }
    myFile.close();
    picNameNum ++;
}
