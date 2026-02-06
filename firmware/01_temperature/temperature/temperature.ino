/*
 * DS18B20 Digital Temperature Sensor Implementation
 *
 * Hardware Interface:
 * - Protocol: OneWire
 * - Pin Assignment: GPIO 4
 * - Power: 3.3V
 * - Requirement: 4.7k Ohm pull-up resistor between DATA and VCC.
 *
 * Software Dependencies:
 * - OneWire Library
 * - DallasTemperature Library
 */

#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 4
#define SERIAL_BAUD 115200

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorAddress;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

  Serial.println("--------------------------------------------------");
  Serial.println("PROXIMAL SENSING - CORE TEMPERATURE MODULE");
  Serial.println("Status: Initializing OneWire Bus...");

  sensors.begin();

  int deviceCount = sensors.getDeviceCount();
  Serial.print("Diagnostic: Found ");
  Serial.print(deviceCount);
  Serial.println(" sensor(s) on bus.");

  if (deviceCount == 0) {
    Serial.println("CRITICAL ERROR: No DS18B20 hardware detected.");
    while (1)
      ;
  }

  if (sensors.getAddress(sensorAddress, 0)) {
    Serial.print("Device Identifier: ");
    for (uint8_t i = 0; i < 8; i++) {
      if (sensorAddress[i] < 16)
        Serial.print("0");
      Serial.print(sensorAddress[i], HEX);
    }
    Serial.println();
  }

  sensors.setResolution(sensorAddress, 12);
  Serial.println("Status: System Ready.");
  Serial.println("--------------------------------------------------");
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensorAddress);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("SYSTEM ALERT: Sensor Communication Lost.");
  } else {
    Serial.print("DATA_POINT: TEMP_C=");
    Serial.print(tempC, 2);

    // Logical evaluation of physiological ranges
    if (tempC >= 37.5) {
      Serial.println(" | STATUS: PYREXIA_ALERT");
    } else if (tempC >= 36.5 && tempC < 37.5) {
      Serial.println(" | STATUS: EUVOMELIA_NORMAL");
    } else {
      Serial.println(" | STATUS: HYPOTHERMIA_WATCH");
    }
  }

  delay(1000); // 1Hz Sampling Rate
}
