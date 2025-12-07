#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME680.h>
#include "secrets.h"

Adafruit_BME680 bme;  // capteur BME680

// -------- WiFi + MQTT --------

WiFiClient espClient;
PubSubClient client(espClient);

// --- Connexion WiFi ---
void setupWifi() {
  Serial.print("Connexion à ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté !");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
}

// --- Reconnect MQTT ---
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Connexion au broker MQTT...");
    if (client.connect("ESP32_BME680")) {
      Serial.println("MQTT connecté !");
    } else {
      Serial.print("Erreur MQTT = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupWifi();
  client.setServer(MQTT_SERVER, 1883);

  // --- Init BME680 ---
  if (!bme.begin()) {
    Serial.println("❌ BME680 introuvable !");
    while (1) delay(100);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  if (!bme.performReading()) {
    Serial.println("Erreur lecture BME680");
    return;
  }

  float temperature = bme.temperature;
  float humidity    = bme.humidity;
  float pressure    = bme.pressure / 100.0;
  float iaq         = bme.gas_resistance / 1000.0;

  // 🔥 Ici, on construit le JSON à la main
  char payload[200];
  snprintf(payload, sizeof(payload),
           "{\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%.2f,\"iaq\":%.2f}",
           temperature, humidity, pressure, iaq);

  client.publish("sensor/env", payload);

  Serial.print("Envoyé MQTT : ");
  Serial.println(payload);

  delay(2000);
}
