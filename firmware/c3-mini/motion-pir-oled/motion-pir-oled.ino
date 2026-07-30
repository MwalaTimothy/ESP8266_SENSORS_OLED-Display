#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1 // Use -1 if there's no reset pin for the OLED
Adafruit_SSD1306 display(OLED_RESET);

const int PIR = 7; // Use a different pin for PIR sensor
int PIRState = 0;

void setup() 
{
  Serial.begin(9600);
  // Initialize display with I2C address 0x3C
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();

  // Initialize PIR sensor pin
  pinMode(PIR, INPUT);
}

void loop() 
{
  PIRState = digitalRead(PIR);

  if (PIRState == HIGH) 
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(35, 15);
    display.print("INTRUDER!");
    display.display();
  } 
  else 
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(35, 15);
    display.print("No Motion");
    display.display();
  }
  
  delay(1000);
}
