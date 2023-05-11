#include <SparkFun_MCP9600.h>

const int NUM_SENSORS = 8;
float errorHandler = -10000.0;

// Assume all sensors use Type K thermocouple
Thermocouple_Type type_s = TYPE_S;
Thermocouple_Type type_k = TYPE_K;

// Initialize array of MCP9600 sensors
MCP9600 sensors[NUM_SENSORS];

// Initialize array of sensor addresses
const uint8_t addresses[NUM_SENSORS] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67};

// Initialize array of sensor enable flags
bool enables[NUM_SENSORS] = {0};

// Initialize array of sensor temperature readings
float temperatures[NUM_SENSORS] = {0};

// Set the type of thermocouple for a given sensor
void setThermoType(int sensorIndex, Thermocouple_Type type) {
    sensors[sensorIndex].setThermocoupleType(type);
}

void init_thermocouples() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i].begin(addresses[i]);
        if (sensors[i].isConnected()) {
            Serial.println("Device will acknowledge! 0x" + String(addresses[i], HEX));
            enables[i] = 1;
        } else {
            Serial.println("Device did not acknowledge! Freezing. 0x" + String(addresses[i], HEX));
            enables[i] = 0;
        }
    }
}

// Check this!!!!!
float get_value(MCP9600 sensor){ //print the thermocouple, ambient and delta temperatures every 200ms if available   
    if(sensor.available())
        return sensor.getThermocoupleTemp();
    return 0;
}

float get_sensor_value(int sensorIndex) {
    if (sensors[sensorIndex].available()) {
        return sensors[sensorIndex].getThermocoupleTemp();
    }
    return errorHandler;
}

float getSensorTemperature(int sensorIndex) {
    return temperatures[sensorIndex];
}

void thermocouple_loop(){
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (enables[i]) {
            float temp = get_sensor_value(i);
            Serial.println("Sensor " + String(i) + ": " + String(temp) + "°C");
            temperatures[i] = temp;
        }
    }
}