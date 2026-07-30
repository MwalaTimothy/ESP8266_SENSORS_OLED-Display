#include "MqttHaDiscovery.h"

#if defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>

namespace CarenuityCore {

namespace {
  WiFiClient wifiClient;
  PubSubClient mqttClient(wifiClient);
}  // namespace

MqttHaDiscovery::MqttHaDiscovery(const char* wifiSsid, const char* wifiPassword,
                                  const char* mqttHost, uint16_t mqttPort,
                                  const char* mqttUser, const char* mqttPassword)
    : _wifiSsid(wifiSsid),
      _wifiPassword(wifiPassword),
      _mqttHost(mqttHost),
      _mqttPort(mqttPort),
      _mqttUser(mqttUser),
      _mqttPassword(mqttPassword) {}

bool MqttHaDiscovery::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_wifiSsid, _wifiPassword);

  uint32_t startMs = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startMs > 15000) {
      return false;  // caller decides how to handle a failed boot (retry, sleep, OLED error, ...)
    }
    delay(250);
  }

  mqttClient.setServer(_mqttHost, _mqttPort);
  return reconnectIfNeeded();
}

bool MqttHaDiscovery::reconnectIfNeeded() {
  if (mqttClient.connected()) return true;

#if defined(ARDUINO_ARCH_ESP32)
  String clientId = "carenuity-" + String((uint32_t)ESP.getEfuseMac(), HEX);
#else
  String clientId = "carenuity-" + String(ESP.getChipId(), HEX);
#endif
  return mqttClient.connect(clientId.c_str(), _mqttUser, _mqttPassword);
}

void MqttHaDiscovery::publishDiscovery(const HaDiscoveryConfig& config) {
  if (!reconnectIfNeeded()) return;

  String topic = String("homeassistant/sensor/") + config.nodeId + "/" + config.objectId + "/config";
  String stateTopic = String("carenuity/") + config.nodeId + "/" + config.objectId + "/state";

  String payload = "{";
  payload += "\"name\":\"" + String(config.name) + "\",";
  payload += "\"unique_id\":\"" + String(config.nodeId) + "_" + config.objectId + "\",";
  payload += "\"state_topic\":\"" + stateTopic + "\"";
  if (config.deviceClass) payload += ",\"device_class\":\"" + String(config.deviceClass) + "\"";
  if (config.unitOfMeasurement) payload += ",\"unit_of_measurement\":\"" + String(config.unitOfMeasurement) + "\"";
  if (config.stateClass) payload += ",\"state_class\":\"" + String(config.stateClass) + "\"";
  payload += ",\"device\":{\"identifiers\":[\"" + String(config.nodeId) + "\"],";
  payload += "\"name\":\"" + String(config.nodeId) + "\",\"manufacturer\":\"Carenuity\"}";
  payload += "}";

  mqttClient.publish(topic.c_str(), payload.c_str(), /*retained=*/true);
}

void MqttHaDiscovery::publishState(const char* nodeId, const char* objectId, const String& value) {
  if (!reconnectIfNeeded()) return;
  String stateTopic = String("carenuity/") + nodeId + "/" + objectId + "/state";
  mqttClient.publish(stateTopic.c_str(), value.c_str());
}

void MqttHaDiscovery::loop() {
  reconnectIfNeeded();
  mqttClient.loop();
}

bool MqttHaDiscovery::isConnected() const {
  return mqttClient.connected();
}

}  // namespace CarenuityCore
