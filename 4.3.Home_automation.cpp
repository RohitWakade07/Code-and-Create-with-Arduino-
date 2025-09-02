#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

WebServer server(80);

// Define relay pins
#define RELAY_LIGHT 5
#define RELAY_FAN 18

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected!");

  // Endpoint to receive commands
  server.on("/execute", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      String body = server.arg("plain");
      Serial.println(body);

      // Parse JSON
      StaticJsonDocument<200> doc;
      deserializeJson(doc, body);

      String device = doc["device"];
      String action = doc["action"];

      if (device == "light") {
        digitalWrite(RELAY_LIGHT, (action == "on") ? LOW : HIGH); // LOW=ON relay
      }
      if (device == "fan") {
        digitalWrite(RELAY_FAN, (action == "on") ? LOW : HIGH);
      }
    }
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
