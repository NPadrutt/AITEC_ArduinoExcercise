/*
 * Random Nerd Tutorials - Rui Santos 
 * Complete Project Details http://randomnerdtutorials.com
 */
 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int buttonPin = 2;

int buttonState = 0;  

void setup()
{  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
  Serial.begin(9600);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop()
{ 
   // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  if (buttonState == HIGH) {
    display.print("Button pressed");  
  } else {
    display.print("Button not pressed");  
  } 
  
  display.display();
}
