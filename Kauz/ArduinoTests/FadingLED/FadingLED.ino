/*
Name:		FadingLed.ino
Created:	12/4/2017 3:33:17 PM
Author:	    Adrian Kauz
Commentary: Tutorial from https://www.arduino.cc/en/Tutorial/Fade
*/

int led = 9;
int brightness = 0;
int fadeAmount = 5;

// the setup function runs once when you press reset or power the board
void setup()
{
    pinMode(led, brightness);
}

// the loop function runs over and over again until power down or reset
void loop()
{
    analogWrite(led, brightness);

    brightness = brightness + fadeAmount;

    if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
    }

    delay(30);
}
