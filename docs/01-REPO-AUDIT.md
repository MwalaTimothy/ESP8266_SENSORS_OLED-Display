# Repository Audit — ESP32-8266_Microcontrolers_Sensors_Actuator

Findings from inspecting the live repo tree (119 tracked paths) and representative sketches
(`SGP30_C3-Mini_0.66-OLED`, `PIR_C3-MINI_0.66-OLED`, `PIR_C3-Mini-BUZZER`,
`BMP180_D1_Mini_0_96_OLED_cloudfree`, `README.md`) on 2026-07-30.

## Structural issues

1. **No `LICENSE` file** — the README claims MIT but no license file exists in the tree. Add one
   or the MIT claim is unenforceable.
2. **Compiled artifacts committed to git.** Every `*_cloudfree` folder and both `PIR_C3-Mini-BUZZER`
   / `SGP30_C3-Mini_0.66-OLED` folders contain a `build/<fqbn>/*.bin|*.elf|*.map` tree (Arduino
   IDE's default output location when "Show verbose output" is on, or PlatformIO-style build dirs
   left in place). `.elf` and `.map` files are pure debug artifacts with no end-user value and
   bloat every clone. Recommendation: `.gitignore` all `build/` directories and instead attach
   final `.bin` files to GitHub Releases when you cut a version.
3. **No shared code.** Every sketch reimplements I2C bring-up, OLED boilerplate, and sensor
   read loops from scratch — copy-paste drift is already visible (see bug list below).
4. **Two different OLED stacks used interchangeably for the same physical shield**:
   `Adafruit_SSD1306` (e.g. `SGP30_C3-Mini_0.66-OLED.ino`, `PIR_C3-MINI_0.66-OLED`) vs. `U8g2lib`
   (e.g. `BMP180_D1_Mini_0_96_OLED_cloudfree.ino`). Pick one per display size and standardize.
5. **Inconsistent naming**: `BH1750_D1-Mini_0.66-OLED` vs `BH1750_D1_Mini_0_96_OLED_cloudfree`
   (hyphen vs underscore, undocumented "cloudfree" suffix meaning "no cloud/MQTT dependency"),
   and one folder with a literal space: `Christmas-Countdown_D1mini_2.8inch display/`. Breaks
   shell globbing and confuses IDE project pickers.
6. **Flat root mixes two different chip families.** ESP8266 (D1-Mini) and ESP32-C3 (C3-Mini)
   sketches sit side by side at the repo root even though they target different cores, toolchains,
   and (per your task brief) are the "IoT Triple Selector" line's two different MCU modules.
7. **Placeholder file** `1.D1-Mini Documentation` contains only the text `# this is a D1` — either
   flesh it out into real docs or remove it; a near-empty file at the repo root looks unfinished.

## Bugs found while reading the code (fix during the refactor, don't carry forward)

- **`PIR_C3-Mini-BUZZER/PIR_Buzzer_C3-Mini.ino`**: PIR is wired to GPIO7 (`pinMode(7, INPUT)`) but
  the trigger check reads `digitalRead(2)` — pin 2 is the buzzer's own output pin, not the PIR
  input. `tone()`/`noTone()` also target pin `3`, which is never configured with `pinMode()`.
  As written, the buzzer cannot be triggered by motion.
- **I2C pin ambiguity on C3-Mini**: `SGP30_C3-Mini_0.66-OLED.ino` explicitly sets
  `Wire.begin(8,10)` (SDA=GPIO8, SCL=GPIO10) for the shield stack, but `PIR_C3-MINI_0.66-OLED`
  calls `Wire.begin()` with no arguments and relies on the ESP32-C3 Arduino core's default pins.
  If those defaults don't match the shield's actual traces, the OLED silently fails on some
  core versions. Every C3-Mini sketch should use the same explicit pin constants (see
  `CarenuityPins.h` in this kit).
- **Confusing comment**: `SGP30_C3-Mini_0.66-OLED.ino` defines `#define OLED_RESET 7 // GPIO0
  corresponds to pin 7 for the C3-mini` — the comment and the value don't obviously agree and
  should be clarified or removed once the shared pin header replaces per-sketch defines.

## What's *not* in the repo yet (despite being requested integration goals)

No sketch currently does Wi-Fi, MQTT, or Home Assistant MQTT Discovery — every `.ino` is a
standalone, serial/OLED-only sketch. That's fine as a starting point, but the README should not
imply HA integration exists today. This kit adds a `MqttHaDiscovery` template as a *starting
point* for the boards that should get connectivity, not a drop-in replacement for existing sketches.
