# Carenuity IoT Triple Selector — Sensor & Actuator Firmware

Modular Arduino firmware for the [Carenuity Solutions Builder](https://www.carenuity.com) "IoT
Triple Selector" shield stack — interchangeable Sensor, Microcontroller, and Actuator/Display
modules built around the **ESP32-C3 Mini** and **D1 Mini (ESP8266)** boards.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Arduino IDE](https://img.shields.io/badge/Arduino-IDE-00979D?logo=arduino&logoColor=white)](https://www.arduino.cc/en/software)
[![ESP32-C3](https://img.shields.io/badge/MCU-ESP32--C3-blue)](https://www.espressif.com/en/products/socs/esp32-c3)
[![ESP8266 D1 Mini](https://img.shields.io/badge/MCU-ESP8266%20D1--Mini-blue)](https://github.com/esp8266/Arduino)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-MQTT%20Discovery-41BDF5?logo=home-assistant&logoColor=white)](docs/home-assistant-integration.md)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

> **Status note:** the sketches in this repo are standalone (serial/OLED output only) unless a
> folder name says otherwise. Wi-Fi / MQTT / Home Assistant auto-discovery is provided as a
> shared, opt-in module (`firmware/common/CarenuityCore/src/MqttHaDiscovery.*`) — see
> [docs/home-assistant-integration.md](docs/home-assistant-integration.md) for which boards use it.

## What's in the box

The Triple Selector system snaps together three module types on a shared shield bus:

| Slot | Examples in this repo |
|---|---|
| **Microcontroller** | ESP32-C3 Mini, D1 Mini (ESP8266) |
| **Sensor** | SGP30 (air quality/eCO2/TVOC), BMP180 (barometric pressure), BH1750 (light), DHT11/DHT22 (temp/humidity), SHT30, DS18B20, HP303B, HC-SR04 (ultrasonic), PIR (motion) |
| **Actuator / Display** | 0.66" OLED, 0.96" OLED, 2.8" TFT, buzzer, button |

Pick one from each column, flash the matching sketch, and the module stack works as a
self-contained sensor or actuator node.

## Hardware pinout — C3-Mini shield stack

The C3-Mini modules share a common I2C bus for the OLED and any I2C sensor. **Use these pins
explicitly in every sketch** — do not rely on `Wire.begin()` defaults, since they are not
guaranteed to match the shield's wiring on every ESP32 Arduino core version.

| Signal | GPIO (C3-Mini) | Notes |
|---|---|---|
| I2C SDA | GPIO8 | Shared bus — OLED + I2C sensors (e.g. SGP30) |
| I2C SCL | GPIO10 | Shared bus |
| PIR sensor (digital in) | GPIO7 | Used by `motion-pir-oled`, `motion-pir-buzzer` |
| Buzzer (PWM / `tone()`) | GPIO2 | Actuator slot |

> Source of truth: `firmware/common/CarenuityCore/src/CarenuityPins.h`. If you change the shield
> wiring, update that header once — every sketch that includes it picks up the change.

## Hardware pinout — D1 Mini shield stack

| Signal | Pin (D1 Mini silkscreen) | GPIO | Notes |
|---|---|---|---|
| I2C SCL | D1 | GPIO5 | OLED + I2C sensors |
| I2C SDA | D2 | GPIO4 | OLED + I2C sensors |

> D1-Mini pin assignments vary more across the older sketches in this repo (see
> [docs/pinouts/d1-mini-pinout.md](docs/pinouts/d1-mini-pinout.md) for the per-sketch table while
> the migration to `CarenuityPins.h` is in progress).

## Getting started

### 1. Hardware

- A Carenuity Triple Selector base board (dual or triple adapter)
- One **microcontroller module**: C3-Mini or D1-Mini
- One **sensor module** and/or **actuator/display module** from the table above
- USB-C (C3-Mini) or Micro-USB (D1-Mini) cable

### 2. Software

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x
- Board support:
  - ESP32-C3: [Espressif Arduino-ESP32 core](https://github.com/espressif/arduino-esp32) via
    Boards Manager (search "esp32", select **ESP32C3 Dev Module**)
  - D1 Mini: [ESP8266 core for Arduino](https://github.com/esp8266/Arduino) via Boards Manager
- Libraries (Library Manager unless noted): `Adafruit GFX Library`, `Adafruit SSD1306`,
  sensor-specific libraries as referenced at the top of each `.ino` (e.g. `Adafruit SGP30`,
  `Adafruit BMP085`, `DHT sensor library`)
- Optional no-IDE route: flash a prebuilt `.bin` from
  [Releases](../../releases) with the [web flashing tool](https://chipglobe-dev.web.app/)

### 3. Flash a sketch

```bash
# Example: air quality node on C3-Mini
1. Open firmware/c3-mini/air-quality-sgp30/air-quality-sgp30.ino in Arduino IDE
2. Tools > Board > ESP32C3 Dev Module
3. Tools > Port > select your C3-Mini's serial port
4. Sketch > Upload
```

### 4. Assemble

1. Connect the sensor and/or actuator module to the microcontroller module on the Triple
   Selector base board.
2. Power via USB.
3. The OLED (if present) starts printing live readings within a few seconds of boot.

## Home Assistant / MQTT integration

Boards under `firmware/*/**-mqtt/` (or any sketch that `#include`s `MqttHaDiscovery.h`) publish
Home Assistant MQTT Discovery payloads on boot, so the entity appears automatically under
**Settings → Devices & Services → MQTT** with no `configuration.yaml` editing required.

Quick start:

1. Have an MQTT broker reachable from your network (Home Assistant's Mosquitto add-on works).
2. Set your Wi-Fi SSID/password and broker host in the sketch's `secrets.h` (see
   `firmware/common/CarenuityCore/src/MqttHaDiscovery.h` for the expected fields —
   **never commit `secrets.h`**, it's already in `.gitignore`).
3. Flash and power the node; it will appear in Home Assistant within a few seconds.

Full topic/payload reference: [docs/home-assistant-integration.md](docs/home-assistant-integration.md).

## Repository layout

See [docs/02-PROPOSED-STRUCTURE.md](docs/02-PROPOSED-STRUCTURE.md) for the full layout and the
naming convention new sketches should follow.

```
firmware/
├── common/CarenuityCore/   # shared pin maps, display driver, MQTT/HA module
├── c3-mini/                # ESP32-C3 sketches
└── d1-mini/                # ESP8266 sketches
```

## Contributing

Adding a new sensor, display, or a brand-new Triple Selector module? See
[CONTRIBUTING.md](CONTRIBUTING.md) — it covers folder/naming conventions, how to use the shared
`CarenuityCore` library instead of copy-pasting boilerplate, and the PR checklist.

## Support this project

If these designs are useful to you, contributions via PayPal to **mwalatimo@gmail.com** are
appreciated. Bug reports, feedback, and pull requests are just as welcome as financial support.

## License

MIT — see [LICENSE](LICENSE).

## Training & partnerships

- [www.chipglobe.com](https://www.chipglobe.com) — internships, training
- [www.carenuity.com](https://www.carenuity.com) — hardware, industrial partnerships
