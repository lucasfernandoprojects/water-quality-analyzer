// ===================================================
// ESP32 Water Quality Analyzer - Calibration Tool
// Created by Lucas Fernando
// Find my work at youtube.com/@lucasfernandochannel
// ===================================================

const int PH_PIN = 34;
const int TDS_PIN = 35;
const int TURBIDITY_PIN = 32;

const float ADC_VREF = 3.3;
const int ADC_RESOLUTION = 4095;

// Voltage divider used on pH sensor
const float PH_DIVIDER_FACTOR = (47.0 + 75.0) / 75.0;
const int NUM_SAMPLES = 100;

// ============================================

int readADC(int pin) {
  long sum = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delay(5);
  }

  return sum / NUM_SAMPLES;
}

// ============================================

float adcToVoltage(int adc) {
  return adc * ADC_VREF / ADC_RESOLUTION;
}

// ============================================

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  delay(2000);

  Serial.println();
  Serial.println("===================================");
  Serial.println("ESP32 SENSOR CALIBRATION TOOL");
  Serial.println("===================================");
}

// ============================================

void loop() {
  int phADC = readADC(PH_PIN);
  int tdsADC = readADC(TDS_PIN);
  int turbidityADC = readADC(TURBIDITY_PIN);

  float phVoltage = adcToVoltage(phADC) * PH_DIVIDER_FACTOR;
  float tdsVoltage = adcToVoltage(tdsADC);
  float turbidityVoltage = adcToVoltage(turbidityADC);

  Serial.println();
  Serial.println("===================================");

  Serial.println("PH SENSOR");
  Serial.print("ADC: ");
  Serial.println(phADC);

  Serial.print("Voltage: ");
  Serial.print(phVoltage, 4);
  Serial.println(" V");

  Serial.println();

  Serial.println("TDS SENSOR");
  Serial.print("ADC: ");
  Serial.println(tdsADC);

  Serial.print("Voltage: ");
  Serial.print(tdsVoltage, 4);
  Serial.println(" V");

  Serial.println();

  Serial.println("TURBIDITY SENSOR");
  Serial.print("ADC: ");
  Serial.println(turbidityADC);

  Serial.print("Voltage: ");
  Serial.print(turbidityVoltage, 4);
  Serial.println(" V");

  Serial.println("===================================");

  delay(5000);
}
