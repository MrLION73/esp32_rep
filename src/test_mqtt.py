import time
import random
import json
import paho.mqtt.client as mqtt

# Configuration du broker
BROKER = "test.mosquitto.org"  # broker public pour test
PORT = 1883
TOPIC = "sensor/env"

# Connexion au broker
client = mqtt.Client()
client.connect(BROKER, PORT)

# Boucle d'envoi de données simulées
while True:
    payload = {
        "temperature": round(20 + random.random()*5, 1),
        "humidity": round(40 + random.random()*10, 1),
        "pressure": round(1000 + random.random()*10, 1),
        "iaq": round(50 + random.random()*50, 1)
    }
    client.publish(TOPIC, json.dumps(payload))
    print(f"Envoyé: {payload}")
    time.sleep(2)  # envoi toutes les 2 secondes
