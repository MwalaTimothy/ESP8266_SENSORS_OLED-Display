// Motion alarm — C3-Mini + PIR + buzzer
// Fixes two bugs present in the original PIR_Buzzer_C3-Mini.ino:
//   1. loop() read digitalRead(2) (the buzzer's own output pin) instead of the PIR
//      input pin, so motion never actually triggered the alarm.
//   2. tone()/noTone() targeted pin 3, which was never configured with pinMode(),
//      instead of the buzzer pin that was actually set up as OUTPUT.
// Pin numbers now come from CarenuityPins.h instead of being hardcoded twice.

#include <CarenuityPins.h>

bool isToneOn = false;
const int frequency = 500;

void setup() {
  pinMode(CarenuityPins::PIR, INPUT);
  pinMode(CarenuityPins::BUZZER, OUTPUT);
}

void loop() {
  if (digitalRead(CarenuityPins::PIR) == HIGH) {
    // Alarm for ~15 seconds (30 * 500ms half-cycles) once motion is detected.
    for (int i = 0; i < 30; i++) {
      if (isToneOn) {
        noTone(CarenuityPins::BUZZER);
        isToneOn = false;
      } else {
        tone(CarenuityPins::BUZZER, frequency);
        isToneOn = true;
      }
      delay(500);
    }
  }
}
