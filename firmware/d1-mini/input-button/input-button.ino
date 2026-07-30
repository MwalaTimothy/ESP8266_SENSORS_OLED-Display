#include <Wire.h>
#include "SSD1306Wire.h"

#define BUTTON_PIN D3 // Pin connected to the button on the shield

SSD1306Wire display(0x3C, SDA, SCL); // Assuming default I2C pins for D1 Mini

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW) { // Button is pressed
    // Check if it's a long press
    if (isLongPress()) {
      displayMessage("Long pressed");
      delay(500);
    } else {// Button is pressed once
      displayMessage("Switch pushed");
      delay(1000);
    }
  } else { // Button is not pressed
    displayMessage("Switch released");
    delay(500);
  }

  delay(100); // Add a small delay to debounce the button
}

bool isLongPress() {
  unsigned long pressTime = millis();
  while (digitalRead(BUTTON_PIN) == LOW) {
    if (millis() - pressTime > 1000) { // Long press duration (adjust as needed)
      return true;
    }
  }
  return false;
}

void displayMessage(String message) {
  display.clear();
  display.drawString(0, 5, "Button Shield");
  display.drawString(0, 30, message);
  display.display();
}
