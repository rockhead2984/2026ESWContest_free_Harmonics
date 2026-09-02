#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "(주)한국전력";
const char* password = "rockhead2984";
const char* mqtt_server = "10.90.93.254";

WiFiClient espClient;
PubSubClient client(espClient);

String inputString = "";

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESP8266Client");
  }
}

void setup() {
  Serial.begin(9600);  // STM32랑 통신
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      client.publish("escalator/data", inputString.c_str());
      inputString = "";
    } else {
      inputString += c;
    }
  }
}