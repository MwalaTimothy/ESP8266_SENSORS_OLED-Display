#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "avdweb_Switch.h"

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

const byte BUTTON_PIN = D3;

Switch button = Switch(BUTTON_PIN);  // Switch between a digital pin and GND

int clickCount = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Examples using Switch library");
  display.display();
}

void loop() {
  button.poll();

  if (button.switched()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Switched");
    display.display();
  }

  if (button.pushed()) {
    clickCount++;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Pushed");
    display.setCursor(0, 10);
    display.print("Click Count: ");
    display.print(clickCount);
    display.display();
  }

  if (button.longPress()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Long Press");
    display.display();
  }

  if (button.doubleClick()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Double Click");
    display.display();
  }

  if (button.released()) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Released");
    display.display();
  }
}
