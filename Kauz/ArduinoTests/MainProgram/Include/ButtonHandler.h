#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

class ButtonHandler
{
    private:
        int _pinNumber = NULL;

    public:
        ButtonHandler();
        ButtonHandler(int);
        void setInputPin(int);
        void waitForButtonPress();
        void waitForButtonPressed();
        bool isPressed();
};

#endif // BUTTON_HANDLER_H