# Contributing

Thanks for helping build out the Carenuity Triple Selector firmware library. This project's
main value is *consistency* — a large pile of one-off sketches is easy to end up with, so please
read this before opening a PR.

## Adding a new sensor/actuator sketch

1. **Pick the right home**: `firmware/c3-mini/<name>/` or `firmware/d1-mini/<name>/`.
2. **Name it `<category>-<part>[-<variant>]`**, lowercase, hyphen-separated, no spaces
   (e.g. `environment-sht30`, `motion-pir-buzzer`). The folder name and the `.ino` filename
   inside it must match exactly — this is an Arduino IDE requirement.
3. **Use the shared library instead of copy-pasting boilerplate.** Include
   `CarenuityPins.h` for pin constants and `DisplayManager.h` for OLED output rather than
   redefining `OLED_RESET`/I2C pins/font setup in every sketch. If the module you're adding needs
   a pin that isn't in `CarenuityPins.h` yet, add it there (one central place beats N sketches
   quietly disagreeing about wiring).
4. **Document the pinout** in the sketch's top-of-file comment block, and update the relevant
   table in `docs/pinouts/`.
5. **Don't commit build artifacts.** `build/`, `*.bin`, `*.elf`, `*.map` are covered by
   `.gitignore` — if `git status` shows one, you're building somewhere it shouldn't be, or your
   IDE's build output path needs fixing.
6. **Never commit credentials.** Wi-Fi/MQTT secrets go in a local `secrets.h` (already
   `.gitignore`'d) with a checked-in `secrets.h.example` template showing the expected fields.

## Adding a brand-new Triple Selector module type

If you're introducing a module category that doesn't fit Sensor/Microcontroller/Actuator-Display
(e.g. a new power/battery module), open an issue first to agree on where it lives in
`firmware/` and whether it needs its own row in the README's module table — this avoids
rework if the layout needs to flex.

## Code style

- One responsibility per sketch's `loop()` — read sensor, format, display/publish. Push shared
  logic into `CarenuityCore`, not into a longer `.ino`.
- Prefer named constants over magic numbers for pins, addresses, and timing (`delay(1000)` is
  fine for a demo sketch; a bare `7` for a GPIO number two files can disagree on is not).
- Match the existing comment density in the file you're editing — light, useful comments, not a
  narration of every line.

## Pull request checklist

- [ ] Sketch compiles for its target board (state which core/version you tested with in the PR)
- [ ] No `build/` output, no secrets, no stray editor files committed
- [ ] Pin usage documented and added to `CarenuityPins.h` if new
- [ ] README module table updated if you added a new sensor/actuator/display type

## Reporting bugs / requesting hardware support

Open a GitHub issue with: which module(s) (MCU + sensor/actuator), Arduino core version, and
what you expected vs. observed. For wiring questions, a photo of your shield stack helps a lot.
