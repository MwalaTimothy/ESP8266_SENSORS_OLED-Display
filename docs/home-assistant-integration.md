# Home Assistant Integration (MQTT Discovery)

Boards that include `CarenuityCore::MqttHaDiscovery` publish [MQTT
Discovery](https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery) payloads on boot, so
entities appear automatically in Home Assistant with no YAML editing.

## Prerequisites

- An MQTT broker reachable from your Wi-Fi network (the Mosquitto broker add-on for Home
  Assistant OS/Supervised is the easiest route).
- Home Assistant's MQTT integration configured and pointed at that broker
  (Settings → Devices & Services → Add Integration → MQTT).

## Per-device setup

1. Copy `firmware/common/CarenuityCore/src/secrets.h.example` to `secrets.h` next to your
   sketch and fill in your Wi-Fi + broker credentials. `secrets.h` is gitignored — never commit it.
2. In `setup()`, construct one `HaDiscoveryConfig` per sensor value you want to expose and call
   `publishDiscovery()` once for each, then call `publishState()` from `loop()` whenever a new
   reading is available. Example for the air-quality node's eCO2 value:

   ```cpp
   CarenuityCore::HaDiscoveryConfig eco2Config = {
     .nodeId = "c3mini-air-quality-01",
     .objectId = "eco2",
     .name = "Air Quality eCO2",
     .deviceClass = "carbon_dioxide",
     .unitOfMeasurement = "ppm",
     .stateClass = "measurement",
   };
   mqtt.publishDiscovery(eco2Config);
   // ...later, in loop():
   mqtt.publishState("c3mini-air-quality-01", "eco2", String(sgp30.eCO2));
   ```

3. Flash and power the node. It should appear under **Settings → Devices & Services → MQTT**
   within a few seconds, grouped under a device named after `nodeId`.

## Topics used

| Purpose | Topic pattern |
|---|---|
| Discovery config (retained) | `homeassistant/sensor/<node_id>/<object_id>/config` |
| State updates | `carenuity/<node_id>/<object_id>/state` |

## Troubleshooting

- **Entity never appears**: confirm the broker sees the discovery publish — subscribe to
  `homeassistant/sensor/#` with an MQTT client (e.g. `mosquitto_sub -t 'homeassistant/#' -v`)
  and power-cycle the node.
- **Entity appears but state never updates**: confirm `mqtt.loop()` is called every `loop()`
  iteration — without it, PubSubClient won't process the connection or send queued publishes.
- **Node keeps reconnecting**: check for a `clientId` collision — every physical device needs a
  unique `nodeId`, since it's used to derive both the HA device identifier and, by extension,
  should stay unique across your fleet.
