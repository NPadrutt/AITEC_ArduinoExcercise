/*
  Name:         ButtonPress.cpp
  Created:      12/11/2017 2:28:02 AM
  Author:       Adrian Kauz
  Commentary:   Manages the ButtonPress Feature
*/

#include <Arduino.h>
#include "ButtonPress.h"

/*
================
setButtonPin()
================
*/
void prepareButton(int buttonPin)
{
    pinMode(buttonPin, INPUT);
}


/*
================
waitForButtonPress()
================
*/
bool waitForButtonPress(int buttonPin)
{
    while (true) {
        if (digitalRead(buttonPin) == HIGH) {
            delay(20);  // Debouncing -> Wait 20ms and check if button is still pressed
            if (digitalRead(buttonPin) == HIGH) {
                return true;
            }
        }
    }
}
