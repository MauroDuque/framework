#include <Arduino.h>
#include <WiFi.h>

int batteryLevel = 0;
const int batteryPin = A0;  // The GPIO pin connected to the battery
float voltage;

void battery_setup() {
    analogReadResolution(12);  // Set ADC resolution to 12 bits
}

int get_battery_percentage() {
    return batteryLevel;
}

int get_voltage_battery() {
    return voltage;
}

void battery_loop() {
    int adcValue = analogRead(batteryPin);
    voltage = adcValue * (3.3 / 4095);  // Convert ADC value to voltage

    // Calculate battery percentage based on voltage readings
    float minVoltage = 0.0;  // Adjust this value based on your battery's specs
    float maxVoltage = 3.2;  // Adjust this value based on your battery's specs
    batteryLevel = map(voltage, minVoltage, maxVoltage, 0, 100);
}
