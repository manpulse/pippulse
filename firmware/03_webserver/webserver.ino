#include "MAX30105.h"
#include "SPIFFS.h"
#include "spo2_algorithm.h"
#include <DallasTemperature.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <OneWire.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <Wire.h>

const char *wifi_ssid = "";
const char *wifi_password = "";
const char *host = "pulsepoint";

#define DS18B20_PIN 4

OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
MAX30105 particleSensor;
spo2_algorithm Spo2;

WebServer server(80);
WebSocketsServer webSocket(81);

uint16_t aun_ir_buffer[100];
uint16_t aun_red_buffer[100];
int8_t n_buffer_count = 0;
int dec = 0;
#define DECIMATION_FACTOR 2

int32_t n_spo2 = 0;
int32_t n_heart_rate = 0;
int8_t ch_spo2_valid = 0;
int8_t ch_hr_valid = 0;
float temperature_c = 0.0;
uint32_t ppg_ir_value = 0;

#define TEMP_READ_INTERVAL 2000
unsigned long last_temp_read = 0;
unsigned long last_ws_push = 0;
#define WS_PUSH_INTERVAL 250

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/"))
    path += "index.html";
  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".htm"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".js"))
    dataType = "application/javascript";
  else if (path.endsWith(".png"))
    dataType = "image/png";
  else if (path.endsWith(".gif"))
    dataType = "image/gif";
  else if (path.endsWith(".jpg"))
    dataType = "image/jpeg";
  else if (path.endsWith(".ico"))
    dataType = "image/x-icon";
  else if (path.endsWith(".xml"))
    dataType = "text/xml";
  else if (path.endsWith(".pdf"))
    dataType = "application/pdf";
  else if (path.endsWith(".zip"))
    dataType = "application/zip";

  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (!dataFile)
    return false;

  server.streamFile(dataFile, dataType);
  dataFile.close();
  return true;
}

void handleRoot() {
  server.sendHeader("Location", "/index.html", true);
  server.send(303, "text/plane", "");
}

void handleNotFound() {
  if (loadFromSpiffs(server.uri()))
    return;
  server.send(404, "text/plain", "File Not Found");
}

void handleVitals() {
  char buf[64];
  sprintf(buf, "%d,%d,%d,%.1f", ch_hr_valid ? (int)n_heart_rate : 0, 0,
          ch_spo2_valid ? (int)n_spo2 : 0, temperature_c);
  server.send(200, "text/html", buf);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
  switch (type) {
  case WStype_CONNECTED:
    Serial.printf("[WS] Client #%u connected\n", num);
    break;
  case WStype_DISCONNECTED:
    Serial.printf("[WS] Client #%u disconnected\n", num);
    break;
  case WStype_TEXT:
    break;
  }
}

void initWiFi() {
  if (strlen(wifi_ssid) > 0) {
    Serial.printf("Connecting to %s", wifi_ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    int wifi_count = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
      wifi_count++;

      if (wifi_count == 60) {
        Serial.println("\nConnection failed, starting SoftAP...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(host);
        Serial.print("AP IP: ");
        Serial.println(WiFi.softAPIP());
        return;
      }
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No WiFi credentials, starting SoftAP...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(host);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  Serial.println("================================");
  Serial.println("  PulsePoint Health Monitor");
  Serial.println("================================\n");

  if (!SPIFFS.begin()) {
    Serial.println("[ERROR] SPIFFS mount failed!");
  } else {
    Serial.println("[OK] SPIFFS mounted");
  }

  initWiFi();

  if (MDNS.begin(host)) {
    Serial.printf("[OK] MDNS: http://%s.local\n", host);
  }

  server.on("/", handleRoot);
  server.on("/vitals", handleVitals);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("[OK] HTTP server on port 80");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("[OK] WebSocket server on port 81");

  ds18b20.begin();
  if (ds18b20.getDeviceCount() > 0) {
    Serial.println("[OK] DS18B20 temperature sensor");
  } else {
    Serial.println("[WARN] DS18B20 not found");
  }

  if (particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    particleSensor.setup(60, 4, 2, 200, 411, 4096);
    Serial.println("[OK] MAX30102 SpO2/HR sensor");
  } else {
    Serial.println("[WARN] MAX30102 not found");
  }

  Serial.println("\n[READY] Open http://" +
                 (WiFi.getMode() == WIFI_AP ? WiFi.softAPIP().toString()
                                            : WiFi.localIP().toString()) +
                 " in browser\n");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  uint32_t ir_raw = particleSensor.getIR();
  uint32_t red_raw = particleSensor.getRed();
  ppg_ir_value = ir_raw;

  bool finger_on = (ir_raw > 50000);

  if (finger_on) {
    dec++;
    if (dec >= DECIMATION_FACTOR) {
      aun_ir_buffer[n_buffer_count] = (uint16_t)(ir_raw >> 4);
      aun_red_buffer[n_buffer_count] = (uint16_t)(red_raw >> 4);
      n_buffer_count++;
      dec = 0;
    }

    if (n_buffer_count > 99) {
      Spo2.estimate_spo2(aun_ir_buffer, 100, aun_red_buffer, &n_spo2,
                         &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
      n_buffer_count = 0;
    }
  } else {
    n_buffer_count = 0;
    dec = 0;
    ch_hr_valid = 0;
    ch_spo2_valid = 0;
  }

  unsigned long now = millis();
  if (now - last_temp_read > TEMP_READ_INTERVAL) {
    ds18b20.requestTemperatures();
    float t = ds18b20.getTempCByIndex(0);
    if (t != DEVICE_DISCONNECTED_C) {
      temperature_c = t;
    }
    last_temp_read = now;
  }

  if (now - last_ws_push > WS_PUSH_INTERVAL) {
    char json[200];
    snprintf(json, sizeof(json),
             "{\"hr\":%d,\"spo2\":%d,\"temp\":%.1f,\"ir\":%lu,"
             "\"hr_valid\":%d,\"spo2_valid\":%d,\"finger\":%d}",
             (int)n_heart_rate, (int)n_spo2, temperature_c,
             (unsigned long)(ir_raw >> 4), (int)ch_hr_valid, (int)ch_spo2_valid,
             finger_on ? 1 : 0);
    webSocket.broadcastTXT(json);
    last_ws_push = now;
  }

  delay(20);
}
