#include <Arduino.h>

const int analogPin = 32; // GPIO34
const int minInput = 0; // Minimum input value
const int maxInput = 3508; // Maximum input value
const int minOutput = 0; // Desired minimum output value
const int maxOutput = 20; // Desired maximum output value
const int numMeasurements = 100; // Number of measurements to average
const unsigned long measurementInterval = 10; // Time between measurements in milliseconds

int totalValue = 0;
int measurementsTaken = 0;

int scaledValue = 0;

float getCurrent() {
  return scaledValue * 1.00;
}

float getScalatedMeasure(){
  return map(scaledValue, 0, 20, 0, 150);
}

int scaleAnalogValue(int analogValue, int minInput, int maxInput, int minOutput, int maxOutput) {
  return map(analogValue, minInput, maxInput, minOutput, maxOutput);
}

void current_4_20MA_setup() {
  pinMode(analogPin, INPUT);
}

void current_4_20MA_loop() {
  int sensorValue = analogRead(analogPin);
  totalValue += sensorValue;
  measurementsTaken++;

  if (measurementsTaken == numMeasurements) {
    int averagedValue = totalValue / numMeasurements;
    scaledValue = scaleAnalogValue(averagedValue, minInput, maxInput, minOutput, maxOutput);
    
    Serial.print("Average analog value on GPIO14: ");
    Serial.print(averagedValue);
    Serial.print(" | Scaled value: ");
    Serial.println(scaledValue);
    
    totalValue = 0;
    measurementsTaken = 0;
  }
}