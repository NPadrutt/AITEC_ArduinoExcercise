#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define FILE_BUFFER_SIZE 48
#define FILE_BASE64_BUFFER_SIZE 72
#define UUID_LENGTH 37                      // UUID V4 defines a length of 36 characters. Another char is used for the terminator '\0'

class HttpHandler
{
    private:
        byte arduinoMACAddr[6];             // Arduino's MAC address
        byte arduinoIpAddr[4];              // Arduino's IP address
        char* serverAddress;
        int  serverPortNr;                  // Port of the webserver
        char fileName[13] = "IMAGE000.JPG";
        char clientID[UUID_LENGTH];

    public:
        void setClientMACAddress(byte, byte, byte, byte, byte, byte);
        void setClientIpAddress(byte, byte, byte, byte);
        void setClientId(char[]);
        void setServerIpAddress(byte, byte, byte, byte);
        void setServerAddress(char*);
        void setServerPort(int);
        void init();
        void postImageToServer(int);
};

#endif // HTTP_HANDLER_H