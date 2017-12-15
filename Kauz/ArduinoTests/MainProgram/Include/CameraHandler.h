#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#define PIC_PKT_LENGTH  128 // Data length of each read

// Use this enum for setting the resolution of the camera
enum
{
    QQQVGA = 0x01,  // 160x120 (80x64)
    QQVGA  = 0x03,  // 160x120
    QVGA   = 0x05,  // 320x240
    VGA    = 0x07   // 640x480
};

class CameraHandler
{
    private:
        byte            _cameraAddress  = NULL;
        unsigned long   _picTotalLength = 0;
        int             _picNumber = 0;

        void clearRxBuf();
        void sendCmd(char[], int);

    public:
        void setAddress(int newCameraAddress);
        void initialize();
        void setResolution(int);
        void captureImage();
        void saveImageToSDCard();
        int getCurrentImageNr();
};

#endif // CAMERA_HANDLER_H