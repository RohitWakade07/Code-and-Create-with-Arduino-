#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// Configuration
#define EEPROM_SIZE 512
#define MAX_DEVICES 10
#define JSON_BUFFER_SIZE 512
#define WIFI_TIMEOUT 30000  // 30 seconds
#define WIFI_RETRY_DELAY 5000  // 5 seconds
#define MAX_RETRY_ATTEMPTS 5

// WiFi credentials - store in EEPROM for production
const char* DEFAULT_SSID = "YOUR_WIFI";
const char* DEFAULT_PASSWORD = "YOUR_PASS";

// Pin definitions
#define RELAY_LIGHT 5
#define RELAY_FAN 18
#define STATUS_LED 2

// Device structure
struct Device {
  String name;
  int pin;
  bool state;
  bool isRelay;
  String type;
};

// Global variables
WebServer server(80);
Device devices[MAX_DEVICES];
int deviceCount = 0;
bool wifiConnected = false;
unsigned long lastWifiCheck = 0;
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 30000; // 30 seconds

// Function prototypes
void setupWiFi();
void reconnectWiFi();
void setupDevices();
void setupEndpoints();
void handleDeviceControl();
void handleDeviceStatus();
void handleSystemStatus();
void handleNotFound();
void updateDeviceState(const String& deviceName, bool state);
bool isValidDevice(const String& deviceName);
void logEvent(const String& event);
void heartbeat();
void saveDeviceStates();
void loadDeviceStates();

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Home Automation System Starting ===");
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  
  // Initialize status LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  
  // Setup devices
  setupDevices();
  
  // Load saved device states
  loadDeviceStates();
  
  // Setup WiFi
  setupWiFi();
  
  // Setup web server endpoints
  setupEndpoints();
  
  Serial.println("=== Home Automation System Ready ===");
}

void setupDevices() {
  // Initialize device array
  devices[0] = {"light", RELAY_LIGHT, false, true, "relay"};
  devices[1] = {"fan", RELAY_FAN, false, true, "relay"};
  deviceCount = 2;
  
  // Set pin modes
  for (int i = 0; i < deviceCount; i++) {
    pinMode(devices[i].pin, OUTPUT);
    digitalWrite(devices[i].pin, HIGH); // Relays are typically HIGH=OFF
    Serial.printf("Device %s initialized on pin %d\n", devices[i].name.c_str(), devices[i].pin);
  }
}

void setupWiFi() {
  Serial.printf("Connecting to WiFi: %s\n", DEFAULT_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_SSID, DEFAULT_PASSWORD);
  
  unsigned long startAttemptTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.printf("\nWiFi connected! IP address: %s\n", WiFi.localIP().toString().c_str());
    digitalWrite(STATUS_LED, HIGH);
    logEvent("WiFi connected successfully");
  } else {
    Serial.println("\nWiFi connection failed!");
    logEvent("WiFi connection failed");
  }
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  
  Serial.println("Attempting to reconnect to WiFi...");
  logEvent("WiFi reconnection attempt");
  
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(DEFAULT_SSID, DEFAULT_PASSWORD);
  
  unsigned long startAttemptTime = millis();
  int retryCount = 0;
  
  while (WiFi.status() != WL_CONNECTED && retryCount < MAX_RETRY_ATTEMPTS) {
    delay(WIFI_RETRY_DELAY);
    retryCount++;
    Serial.printf("Reconnection attempt %d/%d\n", retryCount, MAX_RETRY_ATTEMPTS);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("WiFi reconnected successfully!");
    digitalWrite(STATUS_LED, HIGH);
    logEvent("WiFi reconnected successfully");
  } else {
    wifiConnected = false;
    digitalWrite(STATUS_LED, LOW);
    Serial.println("WiFi reconnection failed!");
    logEvent("WiFi reconnection failed");
  }
}

void setupEndpoints() {
  // Device control endpoint
  server.on("/api/device/control", HTTP_POST, handleDeviceControl);
  
  // Device status endpoint
  server.on("/api/device/status", HTTP_GET, handleDeviceStatus);
  
  // System status endpoint
  server.on("/api/system/status", HTTP_GET, handleSystemStatus);
  
  // Root endpoint
  server.on("/", HTTP_GET, []() {
    String html = "<html><body><h1>Home Automation System</h1>";
    html += "<p>System is running</p>";
    html += "<p>Use /api/device/control to control devices</p>";
    html += "<p>Use /api/device/status to check device states</p>";
    html += "<p>Use /api/system/status to check system status</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
  
  // Handle 404
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void handleDeviceControl() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data provided\"}");
    return;
  }
  
  String body = server.arg("plain");
  Serial.printf("Received command: %s\n", body.c_str());
  
  // Parse JSON with error handling
  StaticJsonDocument<JSON_BUFFER_SIZE> doc;
  DeserializationError error = deserializeJson(doc, body);
  
  if (error) {
    Serial.printf("JSON parsing failed: %s\n", error.c_str());
    server.send(400, "application/json", "{\"error\":\"Invalid JSON format\"}");
    return;
  }
  
  // Validate required fields
  if (!doc.containsKey("device") || !doc.containsKey("action")) {
    server.send(400, "application/json", "{\"error\":\"Missing required fields: device and action\"}");
    return;
  }
  
  String deviceName = doc["device"].as<String>();
  String action = doc["action"].as<String>();
  
  // Validate device
  if (!isValidDevice(deviceName)) {
    server.send(400, "application/json", "{\"error\":\"Invalid device name\"}");
    return;
  }
  
  // Validate action
  if (action != "on" && action != "off" && action != "toggle") {
    server.send(400, "application/json", "{\"error\":\"Invalid action. Use 'on', 'off', or 'toggle'\"}");
    return;
  }
  
  // Execute command
  bool newState = false;
  if (action == "on") {
    newState = true;
  } else if (action == "off") {
    newState = false;
  } else if (action == "toggle") {
    // Find current state and toggle
    for (int i = 0; i < deviceCount; i++) {
      if (devices[i].name == deviceName) {
        newState = !devices[i].state;
        break;
      }
    }
  }
  
  // Update device state
  updateDeviceState(deviceName, newState);
  
  // Save states to EEPROM
  saveDeviceStates();
  
  // Log the action
  logEvent("Device " + deviceName + " turned " + (newState ? "ON" : "OFF"));
  
  // Send success response
  String response = "{\"status\":\"success\",\"device\":\"" + deviceName + "\",\"state\":\"" + (newState ? "on" : "off") + "\"}";
  server.send(200, "application/json", response);
}

void handleDeviceStatus() {
  String response = "{\"devices\":[";
  
  for (int i = 0; i < deviceCount; i++) {
    if (i > 0) response += ",";
    response += "{\"name\":\"" + devices[i].name + "\",";
    response += "\"pin\":" + String(devices[i].pin) + ",";
    response += "\"state\":" + String(devices[i].state ? "true" : "false") + ",";
    response += "\"type\":\"" + devices[i].type + "\"}";
  }
  
  response += "]}";
  server.send(200, "application/json", response);
}

void handleSystemStatus() {
  String response = "{\"system\":{";
  response += "\"wifi_connected\":" + String(wifiConnected ? "true" : "false") + ",";
  response += "\"wifi_strength\":" + String(WiFi.RSSI()) + ",";
  response += "\"ip_address\":\"" + WiFi.localIP().toString() + "\",";
  response += "\"uptime\":" + String(millis() / 1000) + ",";
  response += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
  response += "\"device_count\":" + String(deviceCount);
  response += "}}";
  
  server.send(200, "application/json", response);
}

void handleNotFound() {
  String response = "{\"error\":\"Endpoint not found\",\"available_endpoints\":[";
  response += "\"/api/device/control\",";
  response += "\"/api/device/status\",";
  response += "\"/api/system/status\"";
  response += "]}";
  server.send(404, "application/json", response);
}

void updateDeviceState(const String& deviceName, bool state) {
  for (int i = 0; i < deviceCount; i++) {
    if (devices[i].name == deviceName) {
      devices[i].state = state;
      
      if (devices[i].isRelay) {
        // For relay modules, typically LOW=ON, HIGH=OFF
        digitalWrite(devices[i].pin, state ? LOW : HIGH);
        Serial.printf("Device %s set to %s (pin %d: %s)\n", 
                     deviceName.c_str(), 
                     state ? "ON" : "OFF", 
                     devices[i].pin, 
                     state ? "LOW" : "HIGH");
      }
      break;
    }
  }
}

bool isValidDevice(const String& deviceName) {
  for (int i = 0; i < deviceCount; i++) {
    if (devices[i].name == deviceName) {
      return true;
    }
  }
  return false;
}

void logEvent(const String& event) {
  unsigned long timestamp = millis() / 1000;
  Serial.printf("[%lus] %s\n", timestamp, event.c_str());
}

void heartbeat() {
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    lastHeartbeat = millis();
    
    // Blink status LED to show system is alive
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    
    // Log system status
    Serial.printf("Heartbeat - Free heap: %d bytes, WiFi: %s\n", 
                 ESP.getFreeHeap(), 
                 wifiConnected ? "Connected" : "Disconnected");
  }
}

void saveDeviceStates() {
  int addr = 0;
  for (int i = 0; i < deviceCount; i++) {
    EEPROM.write(addr + i, devices[i].state ? 1 : 0);
  }
  EEPROM.commit();
  Serial.println("Device states saved to EEPROM");
}

void loadDeviceStates() {
  int addr = 0;
  for (int i = 0; i < deviceCount; i++) {
    bool savedState = EEPROM.read(addr + i) == 1;
    devices[i].state = savedState;
    
    // Apply saved state to hardware
    if (devices[i].isRelay) {
      digitalWrite(devices[i].pin, savedState ? LOW : HIGH);
    }
    
    Serial.printf("Loaded state for %s: %s\n", 
                 devices[i].name.c_str(), 
                 savedState ? "ON" : "OFF");
  }
}

void loop() {
  // Handle web server clients
  server.handleClient();
  
  // Check WiFi connection periodically
  if (millis() - lastWifiCheck >= 60000) { // Check every minute
    lastWifiCheck = millis();
    
    if (WiFi.status() != WL_CONNECTED) {
      wifiConnected = false;
      digitalWrite(STATUS_LED, LOW);
      reconnectWiFi();
    }
  }
  
  // System heartbeat
  heartbeat();
  
  // Small delay to prevent watchdog issues
  delay(10);
}
