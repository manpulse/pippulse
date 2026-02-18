#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <Wire.h>

MAX30105 particleSensor;
spo2_algorithm Spo2;

uint16_t aun_ir_buffer[100];
uint16_t aun_red_buffer[100];
int8_t n_buffer_count = 0;

int dec = 0;
#define DECIMATION_FACTOR 2

int32_t n_spo2;
int32_t n_heart_rate;
int8_t ch_spo2_valid;
int8_t ch_hr_valid;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("============================");
  Serial.println("  SpO2 + HR Monitor");
  Serial.println("============================\n");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("[ERROR] MAX30102 not found!");
    while (1)
      ;
  }

  particleSensor.setup(60, 4, 2, 200, 411, 4096);
  Serial.println("Place finger on sensor...\n");
}

void loop() {
  uint32_t ir_raw = particleSensor.getIR();
  uint32_t red_raw = particleSensor.getRed();

  if (ir_raw < 50000) {
    Serial.println("No finger detected");
    n_buffer_count = 0;
    dec = 0;
    delay(100);
    return;
  }

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

    Serial.print("SpO2: ");
    if (ch_spo2_valid) {
      Serial.print(n_spo2);
      Serial.print("%");
    } else {
      Serial.print("--");
    }

    Serial.print("  |  HR: ");
    if (ch_hr_valid) {
      Serial.print(n_heart_rate);
      Serial.print(" BPM");
    } else {
      Serial.print("--");
    }
    Serial.println();

    n_buffer_count = 0;
  }

  delay(20);
}
