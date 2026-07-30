// DisplayManager.h
// Thin wrapper around Adafruit_SSD1306, standardizing OLED setup across sketches
// that currently mix Adafruit_SSD1306 and U8g2lib for the same physical 0.66"/0.96"
// shields. Pick this (or U8g2, but not both) per project.
//
// Usage:
//   CarenuityCore::DisplayManager oled;
//   void setup() { oled.begin(); }
//   void loop() {
//     oled.clear();
//     oled.printLine(0, "CO2: " + String(sgp30.eCO2) + " ppm");
//     oled.printLine(1, "TVOC: " + String(sgp30.TVOC) + " ppb");
//     oled.show();
//   }

#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "CarenuityPins.h"

namespace CarenuityCore {

class DisplayManager {
 public:
  explicit DisplayManager(uint8_t i2cAddress = 0x3C)
      : _display(CarenuityPins::OLED_RESET), _address(i2cAddress) {}

  bool begin() {
    Wire.begin(CarenuityPins::I2C_SDA, CarenuityPins::I2C_SCL);
    if (!_display.begin(SSD1306_SWITCHCAPVCC, _address)) {
      return false;
    }
    _display.setTextColor(WHITE);
    clear();
    return true;
  }

  void clear() { _display.clearDisplay(); }
  void show() { _display.display(); }

  // Prints a line of text at a fixed row (0-indexed, ~12px tall lines at text size 1).
  void printLine(uint8_t row, const String& text, uint8_t textSize = 1) {
    _display.setTextSize(textSize);
    _display.setCursor(0, row * 13);
    _display.print(text);
  }

 private:
  Adafruit_SSD1306 _display;
  uint8_t _address;
};

}  // namespace CarenuityCore
