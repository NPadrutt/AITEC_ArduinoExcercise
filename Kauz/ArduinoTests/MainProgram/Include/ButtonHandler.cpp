/*
  Name:         ButtonHandler.cpp
  Created:      12/11/2017 2:28:02 AM
  Author:       Adrian Kauz
  Commentary:   Manages the ButtonPress Feature
*/

#include <Arduino.h>
#include "ButtonHandler.h"
#include "Globals.h"

/*
================
ButtonHandler()
================
*/
ButtonHandler::ButtonHandler() {}

ButtonHandler::ButtonHandler(int newButtonNumber)
{
    _pinNumber = newButtonNumber;
}


/*
================
setInputPin()
================
*/
void ButtonHandler::setInputPin(int newButtonPin)
{
    pinMode(newButtonPin, INPUT);
    _pinNumber = newButtonPin;
}


/*
================
waitForButtonPress()
================
*/
void ButtonHandler::waitForButtonPress()
{
    DEBUG_PRINT(" - Enter [waitForButtonPress()]");

    while (true) {
        if (digitalRead(_pinNumber) == HIGH) {
            delay(20);  // Debouncing -> Wait 20ms and check if button is still pressed
            if (digitalRead(_pinNumber) == HIGH) {
                DEBUG_PRINT("   - Button pressed!");
                DEBUG_PRINT(" - Leave [waitForButtonPress()]");
                return;
            }
        }
    }
}


/*
================
isPressed()
================
*/
bool ButtonHandler::isPressed()
{
    return (digitalRead(_pinNumber) == HIGH) ? true : false;
}

