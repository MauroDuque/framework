#include <Arduino.h>
#define SHUNT_RESISTOR_VALUE 0.1 // Shunt resistor value in Ohms
#define ADC_BITS 12 // ADC resolution in bits

// 7.3 -> 403 Bites

float current_1 = 0;
float current_2 = 0;

float get_current_sensor(int sensor) {
  if(sensor == 0) return current_1;
  if(sensor == 1) return current_2;
}

void get_current() {
  int rawReading_1 = analogRead(A4); // Read the voltage across the shunt resistor
  int rawReading_2 = analogRead(A5); // Read the voltage across the shunt resistor
  
  float voltageDrop_1 = (rawReading_1 / pow(2, ADC_BITS)) * 3.3; // Convert the analog reading to voltage
  float voltageDrop_2 = (rawReading_2 / pow(2, ADC_BITS)) * 3.3; // Convert the analog reading to voltage
  
  current_1 = (voltageDrop_1 / SHUNT_RESISTOR_VALUE) * 2; // Calculate the current
  current_2 = (voltageDrop_2 / SHUNT_RESISTOR_VALUE) * 2; // Calculate the current
}

void loop_ct() {
  get_current();
  // Serial.println("Current CT1: " + String(current_1) + " A || Current CT2: " + String(current_2) + " A"); // Print the current to the serial monitor
}
