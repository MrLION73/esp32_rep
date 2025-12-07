#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME680.h>
#include <iostream>

Adafruit_BME680 bme; // capteur BME680

const char* ssid = "TON_WIFI";
const char* password = "TON_MDP";
const char* mqtt_server = "test.mosquitto.org"; // broker public temporaire

WiFiClient espClient;
PubSubClient client(espClient);

float getFakeTemperature() { return 20 + random(0, 100)/10.0; }
float getFakeHumidity()    { return 40 + random(0, 100)/10.0; }
float getFakePressure()    { return 1000 + random(0, 50)/10.0; }
float getFakeIAQ()         { return 50 + random(0, 50)/10.0; }

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);
}

void loop() {
  float temp = getFakeTemperature();
  float hum  = getFakeHumidity();
  float pres = getFakePressure();
  float iaq  = getFakeIAQ();
  std::cout << "Temp: " << temp << " °C, Hum: " << hum << " %" << std::endl;
  delay(1000);
}
