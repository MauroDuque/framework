#include <Arduino.h>
#define SHUNT_RESISTOR_VALUE 0.1 // Shunt resistor value in Ohms
#define ADC_BITS 12 // ADC resolution in bits
#define VOLTAGE_SHUNT 7.3 // Voltage across the shunt resistor in volts
#define VOLTAGE 220 // Voltage transformation
float phase_factor = 1.73; // √3 Three phases root(3)

float current_1;
float current_2;
float power_1_phase;
float power_2_phase;
float total_power;

int CT_1 = A4;
int CT_2 = A5;

float get_current_1() {
  return current_1;
}

float get_current_2() {
  return current_2;
}

float get_power_1() {
  return power_1_phase;
}

float get_power_2() {
  return power_2_phase;
}

float get_total_power() {
  return total_power;
}

float get_current(int CT) {
  int rawReading = analogRead(CT); // Read the voltage across the shunt resistor
  float voltageDrop = (rawReading / pow(2, ADC_BITS)) * VOLTAGE_SHUNT; // Convert the analog reading to voltage
  float current = voltageDrop / SHUNT_RESISTOR_VALUE; // Calculate the current
  return current;
}

float calculate_power(float current) {
  float power = phase_factor * current * VOLTAGE; // Calculate power (P = IV)
  return power;
}

float calculate_total_power(float current) {
  float power = 3 * phase_factor * current * VOLTAGE; // Calculate power (P = IV)
  return power;
}

void loop_ct() {
  current_1 = get_current(CT_1);
  current_2 = get_current(CT_2);

  power_1_phase = calculate_power(current_1);
  power_2_phase = calculate_power(current_2);

  total_power = calculate_total_power(current_1);

  Serial.print("Current CT1: " + String(current_1) + " A || Current CT2: " + String(current_2) + " A");
  Serial.print(" || Power CT1: " + String(power_1_phase) + " W || Power CT2: " + String(power_2_phase) + " W");
  Serial.println(" || Total Power: " + String(total_power) + " W");
}
