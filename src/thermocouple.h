#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include <SparkFun_MCP9600.h>
#include <Arduino_JSON.h>

void init_thermocouples();
float get_sensor_value(int sensorIndex);
float getSensorTemperature(int sensorIndex);
void thermocouple_loop();
#endif