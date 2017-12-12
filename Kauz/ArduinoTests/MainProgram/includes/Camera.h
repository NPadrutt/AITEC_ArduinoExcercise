#ifndef CAMERA_H
#define CAMERA_H

#define PIC_PKT_LENGTH  128 // Data length of each read

// Use this enum for setting the resolution of the camera
enum
{
    QQQVGA = 0x01,  // 80x64
    QQVGA  = 0x03,  // 160x128
    QVGA   = 0x05,  // 320x240
    VGA    = 0x07   // 640x480
};

class Camera
{
    private:
        byte            cameraAddress  = NULL;
        unsigned long   picTotalLength = 0;
        int             picNumber = 0;

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

#endif // CAMERA_H