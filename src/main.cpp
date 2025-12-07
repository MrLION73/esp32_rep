#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME680.h>
#include "secrets.h"
#include <Wire.h>

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

  // I2C sur les pins ESP32
  Wire.begin(21, 22);

  Serial.println("Scan I2C...");
  delay(2000);

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan terminé.");
}

void loop() {}