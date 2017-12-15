/*
  Name:         Camera.cpp
  Created:      12/11/2017 2:28:02 AM
  Author:       Adrian Kauz
  Built on:     "SerialCamera_DemoCode_CJ-OV528.ino" by Jack Shao 8/8/2013
  Commentary:   Manages the Camera Feature.
*/

#include <SPI.h>
#include <SD.h>
#include "CameraHandler.h"
#include "Globals.h"

/*
================
Camera()
================
*/
void CameraHandler::setAddress(int newCameraAddress)
{
    DEBUG_PRINT(" - Enter [CameraHandler::setAddress()]");

    _cameraAddress = (newCameraAddress << 5);

    DEBUG_PRINT(" - Leave [CameraHandler::setAddress()]");
}


/*
================
clearRxBuf()
================
*/
void CameraHandler::clearRxBuf()
{
    while (Serial.available())
    {
        Serial.read();
    }
}


/*
================
sendCmd()
================
*/
void CameraHandler::sendCmd(char cmd[], int cmd_len)
{
    for (char i = 0; i < cmd_len; i++) {
        Serial.print(cmd[i]);
    }
}


/*
================
initialize()
================
*/
void CameraHandler::initialize()
{
    DEBUG_PRINT(" - Enter [CameraHandler::initialize()]");

    char cmd[] = { 0xaa,0x0d | _cameraAddress,0x00,0x00,0x00,0x00 };
    unsigned char resp[6];

    Serial.setTimeout(500);

    while (1) {
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) {
            continue;
        }

        if (resp[0] == 0xaa && resp[1] == (0x0e | _cameraAddress) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0) {
            if (Serial.readBytes((char *)resp, 6) != 6) {
                continue;
            }

            if (resp[0] == 0xaa && resp[1] == (0x0d | _cameraAddress) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) {
                break;
            }
        }
    }

    cmd[1] = 0x0e | _cameraAddress;
    cmd[2] = 0x0d;
    sendCmd(cmd, 6);

    DEBUG_PRINT("\n - Leave [CameraHandler::initialize()]");
}


/*
================
setResolution()
================
*/
void CameraHandler::setResolution(int resolution)
{
    DEBUG_PRINT(" - Enter [CameraHandler::setResolution()]");

    char cmd[] = { 0xaa, 0x01 | _cameraAddress, 0x00, 0x07, 0x00, resolution };
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);

        if (Serial.readBytes((char *)resp, 6) != 6) {
            continue;
        }

        if (resp[0] == 0xaa && resp[1] == (0x0e | _cameraAddress) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) {
            break;
        }
    }

    DEBUG_PRINT("\n - Leave [CameraHandler::setResolution()]");
}


/*
================
captureImage()
================
*/
void CameraHandler::captureImage()
{
    DEBUG_PRINT(" - Enter [CameraHandler::captureImage()]");

    char cmd[] = { 0xaa, 0x06 | _cameraAddress, 0x08, PIC_PKT_LENGTH & 0xff, (PIC_PKT_LENGTH >> 8) & 0xff ,0 };
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1) {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) {
            continue;
        }
        
        if (resp[0] == 0xaa && resp[1] == (0x0e | _cameraAddress) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) {
            break;
        }
    }

    cmd[1] = 0x05 | _cameraAddress;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = 0;
    
    while (1) {
        clearRxBuf();
        sendCmd(cmd, 6);

        if (Serial.readBytes((char *)resp, 6) != 6) {
            continue;
        }

        if (resp[0] == 0xaa && resp[1] == (0x0e | _cameraAddress) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) {
            break;
        }
    }

    cmd[1] = 0x04 | _cameraAddress;
    cmd[2] = 0x1;
    
    while (1) {
        clearRxBuf();
        sendCmd(cmd, 6);

        if (Serial.readBytes((char *)resp, 6) != 6) {
            continue;
        }

        if (resp[0] == 0xaa && resp[1] == (0x0e | _cameraAddress) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0) {
            Serial.setTimeout(1000);
            
            if (Serial.readBytes((char *)resp, 6) != 6)
            {
                continue;
            }
            
            if (resp[0] == 0xaa && resp[1] == (0x0a | _cameraAddress) && resp[2] == 0x01)
            {
                _picTotalLength = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
                break;
            }
        }
    }

    DEBUG_PRINT("\n - Leave [CameraHandler::captureImage()]");
}


/*
================
saveImageToSDCard()
================
*/
void CameraHandler::saveImageToSDCard()
{
    DEBUG_PRINT(" - Enter [CameraHandler::saveImageToSDCard()]");
    
    File myFile;

    unsigned int pktCnt = (_picTotalLength) / (PIC_PKT_LENGTH - 6);
    
    if ((_picTotalLength % (PIC_PKT_LENGTH - 6)) != 0) {
        pktCnt += 1;
    }

    char cmd[] = { 0xaa, 0x0e | _cameraAddress, 0x00, 0x00, 0x00, 0x00 };
    unsigned char pkt[PIC_PKT_LENGTH];
    
    char picName[] = "IMAGE000.JPG";
    picName[5] = _picNumber / 100 + '0';
    picName[6] = _picNumber / 10 + '0';
    picName[7] = _picNumber % 10 + '0';

    if (SD.exists(picName))
    {
        SD.remove(picName);
    }

    myFile = SD.open(picName, FILE_WRITE);

    if (myFile) {
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
                uint16_t cnt = Serial.readBytes((char *)pkt, PIC_PKT_LENGTH);
                unsigned char sum = 0;

                for (int y = 0; y < cnt - 2; y++)
                {
                    sum += pkt[y];
                }

                if (sum != pkt[cnt - 2])
                {
                    if (++retry_cnt < 100) {
                        goto retry;
                    }
                    else {
                        break;
                    }
                }

                myFile.write((const uint8_t *)&pkt[4], cnt - 6);
        }

        cmd[4] = 0xf0;
        cmd[5] = 0xf0;
        
        sendCmd(cmd, 6);
    }

    myFile.flush();
    myFile.close();
    _picNumber++;

    DEBUG_PRINT("\n - Leave [CameraHandler::saveImageToSDCard()]");
}


/*
================
saveImageToSDCard()
================
*/
int CameraHandler::getCurrentImageNr()
{
    return _picNumber - 1;
}