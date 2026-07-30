// MqttHaDiscovery.h
// Opt-in Wi-Fi + MQTT + Home Assistant MQTT Discovery helper.
//
// This is a STARTING TEMPLATE, not something wired into any existing sketch yet —
// none of the current .ino files in this repo do networking. Include this in a new
// "*-mqtt" sketch variant when you want a sensor to show up in Home Assistant
// automatically under Settings > Devices & Services > MQTT.
//
// Requires the "PubSubClient" library (Library Manager).
//
// Wi-Fi/broker credentials must NOT be hardcoded here or in the sketch — create a local
// secrets.h (see secrets.h.example in this folder) that is .gitignore'd.
//
// HA MQTT Discovery protocol reference:
// https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery

#pragma once

#include <Arduino.h>

namespace CarenuityCore {

struct HaDiscoveryConfig {
  const char* nodeId;        // unique per physical device, e.g. "c3mini-air-quality-01"
  const char* objectId;      // unique per entity on that device, e.g. "eco2"
  const char* name;          // friendly name shown in HA, e.g. "Air Quality eCO2"
  const char* deviceClass;   // HA device_class, e.g. "carbon_dioxide", "temperature" (nullable)
  const char* unitOfMeasurement; // e.g. "ppm", "°C" (nullable)
  const char* stateClass;    // "measurement" for sensors that report a running value
};

class MqttHaDiscovery {
 public:
  // wifiSsid/wifiPassword/mqttHost should come from secrets.h, not literals in the sketch.
  MqttHaDiscovery(const char* wifiSsid, const char* wifiPassword,
                   const char* mqttHost, uint16_t mqttPort,
                   const char* mqttUser, const char* mqttPassword);

  // Connects Wi-Fi and the MQTT client. Call once from setup(), after Serial.begin().
  bool begin();

  // Publishes the HA discovery config payload for one entity. Call once per entity in
  // setup(), after begin() succeeds.
  void publishDiscovery(const HaDiscoveryConfig& config);

  // Publishes a state value for a previously-discovered entity. Call from loop().
  void publishState(const char* nodeId, const char* objectId, const String& value);

  // Keeps the MQTT connection alive; call every loop() iteration.
  void loop();

  bool isConnected() const;

 private:
  bool reconnectIfNeeded();

  const char* _wifiSsid;
  const char* _wifiPassword;
  const char* _mqttHost;
  uint16_t _mqttPort;
  const char* _mqttUser;
  const char* _mqttPassword;
};

}  // namespace CarenuityCore
