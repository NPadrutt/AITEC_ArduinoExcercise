#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#define FILE_BUFFER_SIZE 48  // Must be a multiple of 3!
#define FILE_BASE64_BUFFER_SIZE (FILE_BUFFER_SIZE / 3 * 4) + 1

class HttpHandler
{
    private:
        byte  _arduinoMACAddr[6];             // Arduino's MAC address
        byte  _arduinoIpAddr[4];              // Arduino's IP address
        char* _serverAddress;
        int   _serverPortNr;                  // Port of the webserver
        char  _fileName[13] = "IMAGE000.JPG";
        char* _clientUUID;

        bool _matchString(char*, char*, int);

    public:
        void setClientMACAddress(byte, byte, byte, byte, byte, byte);
        void setClientIpAddress(byte, byte, byte, byte);
        void setClientUUID(char*);
        void setServerAddress(char*);
        void setServerPort(int);
        void init();
        void postImageToServer(int);
        bool getReplyFromServer(char*, int);
};

#endif // HTTP_HANDLER_H