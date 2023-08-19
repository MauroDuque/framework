#ifndef BATTERY_H
#define BATTERY_H

void battery_setup();
void battery_loop();
int get_battery_percentage();
int get_voltage_battery();

#endif