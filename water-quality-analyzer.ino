// ===================================================
// ESP32 Water Quality Analyzer
// Fully Calibrated On The Test Sketch
// Created by Lucas Fernando
// Find my work at youtube.com/@lucasfernandochannel
// ===================================================

// ---------------- PIN DEFINITIONS ----------------
const int PH_PIN = 34;
const int TDS_PIN = 35;
const int TURBIDITY_PIN = 32;

// ---------------- ADC SETTINGS ----------------
const float ADC_VREF = 3.3;
const int ADC_RESOLUTION = 4095;

// ---------------- SAMPLING ----------------
const int NUM_SAMPLES = 20;

// ============================================
// pH SENSOR
// ============================================
const float PH_DIVIDER_FACTOR = (47.0 + 75.0) / 75.0;

// --------------------------------------------
// pH Calibration
// Measured values:
// pH 4  -> 1.193V
// pH 7  -> 1.053V
// pH 10 -> 0.939V
// --------------------------------------------
const float PH_SLOPE = -23.6;
const float PH_OFFSET = 32.2;

// ============================================
// TDS SENSOR
// ============================================
// Calibration:
// 1413 µS/cm solution

const float EC_SCALE_FACTOR = 1010.0;
const float TDS_FACTOR = 0.5;

// ============================================
// READ AVERAGED ADC
// ============================================

int readADC(int pin) {
  long sum = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delay(10);
  }

  return sum / NUM_SAMPLES;
}

// ============================================
// ADC TO VOLTAGE
// ============================================

float adcToVoltage(int adc) {
  return (adc * ADC_VREF) / ADC_RESOLUTION;
}

// ============================================
// WATER QUALITY CLASSIFICATION
// ============================================

String classifyWater(float pH, float tds, float turbidityVoltage) {
  int pollutionScore = 0;

  // --------------------------------------------
  // pH ANALYSIS
  // --------------------------------------------

  if (pH < 5.0 || pH > 9.0) {
    pollutionScore += 3;
  } else if (pH < 6.5 || pH > 8.5) {
    pollutionScore += 2;
  }

  // --------------------------------------------
  // TDS ANALYSIS
  // --------------------------------------------

  if (tds > 1000) {
    pollutionScore += 3;
  } else if (tds > 500) {
    pollutionScore += 2;
  } else if (tds > 300) {
    pollutionScore += 1;
  }

  // --------------------------------------------
  // TURBIDITY ANALYSIS
  // --------------------------------------------
  // Based on test measurements:
  // 0.283V = cleaner sample
  // 0.875V = dirtier sample
  //
  // Therefore:
  // Higher voltage = higher turbidity
  // --------------------------------------------

  if (turbidityVoltage > 1.20) {
    pollutionScore += 3;
  } else if (turbidityVoltage > 0.80) {
    pollutionScore += 2;
  } else if (turbidityVoltage > 0.40) {
    pollutionScore += 1;
  }

  // --------------------------------------------
  // FINAL CLASSIFICATION
  // --------------------------------------------

  if (pollutionScore <= 1) {
    return "CLEAN";
  } else if (pollutionScore <= 3) {
    return "MODERATE";
  } else if (pollutionScore <= 6) {
    return "POLLUTED";
  } else {
    return "VERY POLLUTED";
  }
}

// ============================================
// SETUP
// ============================================

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("ESP32 Water Quality Analyzer");
  Serial.println("Fully Calibrated Test");
  Serial.println("======================================");
}

// ============================================
// MAIN LOOP
// ============================================

void loop() {
  // ============================================
  // READ ADC VALUES
  // ============================================

  int phADC = readADC(PH_PIN);
  int tdsADC = readADC(TDS_PIN);
  int turbidityADC = readADC(TURBIDITY_PIN);

  // ============================================
  // CONVERT TO VOLTAGES
  // ============================================

  float phVoltage = adcToVoltage(phADC) * PH_DIVIDER_FACTOR;
  float tdsVoltage = adcToVoltage(tdsADC);
  float turbidityVoltage = adcToVoltage(turbidityADC);

  // ============================================
  // pH CALCULATION
  // ============================================

  float pHValue = (PH_SLOPE * phVoltage) + PH_OFFSET;

  if (pHValue < 0) {
    pHValue = 0;
  } else if (pHValue > 14) {
    pHValue = 14;
  }

  // ============================================
  // TDS / CONDUCTIVITY
  // ============================================

  float conductivity_uS = tdsVoltage * EC_SCALE_FACTOR;
  float tdsPPM = conductivity_uS * TDS_FACTOR;

  // ============================================
  // TURBIDITY STATE
  // ============================================

  String turbidityState;

  if (turbidityVoltage < 0.40) {
    turbidityState = "CLEAN";
  } else if (turbidityVoltage < 0.80) {
    turbidityState = "MODERATE";
  } else if (turbidityVoltage < 1.20) {
    turbidityState = "POLLUTED";
  } else {
    turbidityState = "VERY POLLUTED";
  }

  // ============================================
  // GLOBAL WATER QUALITY
  // ============================================

  String waterQuality = classifyWater(pHValue, tdsPPM, turbidityVoltage);

  // ============================================
  // PRINT RESULTS
  // ============================================

  Serial.println();
  Serial.println("======================================");

  // --------------------------------------------
  // pH SENSOR
  // --------------------------------------------

  Serial.println("pH SENSOR");

  Serial.print("ADC: ");
  Serial.println(phADC);

  Serial.print("Voltage: ");
  Serial.print(phVoltage, 3);
  Serial.println(" V");

  Serial.print("Estimated pH: ");
  Serial.println(pHValue, 2);

  // --------------------------------------------
  // TDS SENSOR
  // --------------------------------------------

  Serial.println();
  Serial.println("TDS SENSOR");

  Serial.print("ADC: ");
  Serial.println(tdsADC);

  Serial.print("Voltage: ");
  Serial.print(tdsVoltage, 3);
  Serial.println(" V");

  Serial.print("Conductivity: ");
  Serial.print(conductivity_uS, 0);
  Serial.println(" uS/cm");

  Serial.print("Estimated TDS: ");
  Serial.print(tdsPPM, 0);
  Serial.println(" ppm");

  // --------------------------------------------
  // TURBIDITY SENSOR
  // --------------------------------------------

  Serial.println();
  Serial.println("TURBIDITY SENSOR");

  Serial.print("ADC: ");
  Serial.println(turbidityADC);

  Serial.print("Voltage: ");
  Serial.print(turbidityVoltage, 3);
  Serial.println(" V");

  Serial.print("Turbidity Level: ");
  Serial.println(turbidityState);

  // --------------------------------------------
  // FINAL WATER QUALITY
  // --------------------------------------------

  Serial.println();

  Serial.print("FINAL WATER QUALITY: ");
  Serial.println(waterQuality);

  // ============================================

  delay(2000);
}
