// Air quality node — C3-Mini + SGP30 + 0.66" OLED
// Refactor of the original SGP30_C3-Mini_0.66-OLED.ino, using the shared
// CarenuityCore library for pins and display setup instead of local #defines.

#include <Adafruit_SGP30.h>
#include <CarenuityPins.h>
#include <DisplayManager.h>

Adafruit_SGP30 sgp30;
CarenuityCore::DisplayManager oled;

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println("OLED init failed");
    while (true) { delay(1000); }
  }

  if (!sgp30.begin()) {
    oled.clear();
    oled.printLine(0, "SGP30 not found");
    oled.show();
    while (true) { delay(1000); }
  }
}

void loop() {
  delay(1000);

  if (sgp30.IAQmeasure()) {
    oled.clear();
    oled.printLine(0, "CO2: " + String(sgp30.eCO2) + " ppm");
    oled.printLine(1, "TVOC: " + String(sgp30.TVOC) + " ppb");
    oled.show();
  } else {
    oled.clear();
    oled.printLine(0, "Read error");
    oled.show();
  }
}
