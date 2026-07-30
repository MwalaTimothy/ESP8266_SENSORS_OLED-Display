// CarenuityPins.h
// Single source of truth for Triple Selector shield-stack wiring.
// Include the block for whichever microcontroller module the sketch targets.
//
// Why this exists: SGP30_C3-Mini_0.66-OLED.ino called Wire.begin(8,10) explicitly,
// while PIR_C3-MINI_0.66-OLED.ino called Wire.begin() with no arguments and relied on
// core defaults. Centralizing the pin numbers here means every sketch agrees, and a
// wiring change only needs to happen in one place.

#pragma once

#if defined(ARDUINO_ARCH_ESP32)
// ---- C3-Mini module ----------------------------------------------------
namespace CarenuityPins {
  constexpr int I2C_SDA   = 8;   // shared bus: OLED + I2C sensors (SGP30, etc.)
  constexpr int I2C_SCL   = 10;
  constexpr int PIR       = 7;   // motion sensor digital input
  constexpr int BUZZER    = 2;   // actuator slot, drive with tone()/noTone()
  constexpr int OLED_RESET = -1; // most 0.66"/0.96" OLED shields have no reset pin wired
}

#elif defined(ARDUINO_ARCH_ESP8266)
// ---- D1-Mini module ------------------------------------------------------
namespace CarenuityPins {
  constexpr int I2C_SCL   = D1;  // GPIO5
  constexpr int I2C_SDA   = D2;  // GPIO4
  constexpr int OLED_RESET = -1;

  // TODO: DHT11/DHT22, DS18B20, HC-SR04, and button pins vary across the existing
  // D1-Mini sketches and haven't been unified yet. As each sketch is migrated onto
  // CarenuityCore, move its verified pin(s) here instead of leaving them as local
  // #defines, so this file stays the one place to check shield wiring.
}

#else
  #error "CarenuityPins.h: unsupported architecture — add a block for this board."
#endif
