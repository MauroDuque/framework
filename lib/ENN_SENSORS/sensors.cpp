#include <Arduino.h>
#include "battery.h"
// #include "settings.h"

void init_sensors(){
    battery_setup();
}

void read_sensors() {
    battery_loop();
}
