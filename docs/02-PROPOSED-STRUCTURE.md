# Proposed Directory Layout

Goals: browsable by chip family first (they don't share a toolchain), shared driver code lives
in exactly one place, build artifacts never enter git, and every leaf folder is still a normal
Arduino-IDE-openable sketch (no forced migration to PlatformIO, though it's noted as an upgrade
path below).

```
ESP32-8266_Microcontrolers_Sensors_Actuator/
├── README.md
├── LICENSE
├── CONTRIBUTING.md
├── .gitignore
│
├── docs/
│   ├── pinouts/
│   │   ├── c3-mini-pinout.md
│   │   └── d1-mini-pinout.md
│   ├── home-assistant-integration.md
│   └── images/                      # wiring photos, board renders used by README
│
├── schematics/
│   └── triple-selector/             # Fritzing/KiCad/PDF wiring diagrams for the shield stack
│
├── firmware/
│   ├── common/
│   │   └── CarenuityCore/           # shared Arduino library (see below)
│   │       ├── library.properties
│   │       └── src/
│   │           ├── CarenuityPins.h        # per-board pin maps, one block per MCU
│   │           ├── DisplayManager.h/.cpp  # single OLED wrapper (retires the SSD1306/U8g2 split)
│   │           └── MqttHaDiscovery.h/.cpp # opt-in Wi-Fi + MQTT + HA autodiscovery
│   │
│   ├── c3-mini/                     # ESP32-C3 RISC-V sketches
│   │   ├── air-quality-sgp30/
│   │   │   └── air-quality-sgp30.ino
│   │   ├── motion-pir-oled/
│   │   │   └── motion-pir-oled.ino
│   │   └── motion-pir-buzzer/
│   │       └── motion-pir-buzzer.ino
│   │
│   └── d1-mini/                     # ESP8266 sketches
│       ├── environment-bmp180/
│       ├── environment-bh1750/
│       ├── environment-dht11/
│       ├── environment-dht22/
│       ├── environment-ds18b20/
│       ├── environment-sht30/
│       ├── environment-sgp30/
│       ├── environment-hp303b/
│       ├── input-button/
│       ├── input-hcsr04-ultrasonic/
│       ├── motion-pir-oled/
│       ├── motion-pir-buzzer/
│       └── christmas-countdown-tft28/
│
└── archive/
    └── legacy-cloudfree/            # old "*_cloudfree" duplicates, kept for history only
```

## Naming convention

`<category>-<primary-part>[-<variant>]`, all lowercase, hyphen-separated, no spaces:

- `category`: `environment`, `air-quality`, `motion`, `input`, `display-demo`
- `primary-part`: the sensor/module driving the sketch (`bmp180`, `sgp30`, `pir`, `hcsr04`)
- One sketch = one folder = one `.ino` with the same basename as the folder (Arduino IDE
  requirement).

## Handling the "cloudfree" duplicates

You currently have two versions of several sensors: a bare version
(`BH1750_D1-Mini_0.66-OLED`) and a `_cloudfree` version with a `build/` folder checked in. Pick
one of two paths and apply it consistently rather than keeping both indefinitely:

- **If `_cloudfree` is the actively maintained one**: it becomes the canonical
  `firmware/d1-mini/environment-bh1750/`, and the older duplicate moves to `archive/legacy-cloudfree/`
  (or is deleted if it's truly superseded — check git blame/history first).
- **If both are meant to diverge on purpose** (e.g. one talks to MQTT, one doesn't): rename them
  to make the distinction explicit in the folder name, e.g. `environment-bh1750/` (standalone) and
  `environment-bh1750-mqtt/` (networked), instead of the unexplained `_cloudfree` suffix.

## Upgrade path: PlatformIO (optional, not required)

Because this repo spans two chip families (ESP8266 + ESP32-C3) that need different
`platform =` / `board =` values, PlatformIO is a natural fit if the project grows further:
a single `platformio.ini` can define one `[env:c3mini]` and one `[env:d1mini]`, both pointing
`lib_extra_dirs` at `firmware/common/CarenuityCore`, so shared code is picked up without any
Arduino IDE "install this .zip library" step. This is a nice-to-have, not a blocker — the
layout above works unmodified in the Arduino IDE today.
