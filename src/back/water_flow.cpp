#include <Arduino.h>

const int sensorPin = 13;  // Pin where the FL-808 water flow sensor is connected
int SAMPLING_PERIOD_WATER = 60 * 1000;
unsigned long TIME_SAMPLING_PERIOD_WATER = 0;
int flowState = 0;
float pulsePerLiter = 770;
int previusStateFlow = 0;
float counterFlow = 0.0;
float volume = 0.0;

float get_liters_per_minute() {
    return (counterFlow / pulsePerLiter);
}

float get_total_volume() { // liters
    float liters = get_liters_per_minute();
    volume = volume + liters;
    return volume;
}

void setup_water_flow() {
    pinMode(sensorPin, INPUT);
}

void loop_water_flow() {
    int flowState = digitalRead(sensorPin);
    if(flowState != previusStateFlow) {
        previusStateFlow = flowState;
        counterFlow = counterFlow + 1.0;
    }

    if(millis() >= TIME_SAMPLING_PERIOD_WATER + SAMPLING_PERIOD_WATER) {
        TIME_SAMPLING_PERIOD_WATER += SAMPLING_PERIOD_WATER;
        get_total_volume();
        counterFlow = 0.0;
    } 
}