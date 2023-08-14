#include <Arduino.h>
#include "Enerinno_MCP9600.h"
#include "settings.h"
#include "ct.h"
#include "humidity.h"
#include "GAS.h"
#include "water_flow.h"
#include "tmp_102.h"
#include "temp_max.h"
#include "Current_4_20mA.h"
#include "battery.h"
#include "strengthRSSI.h"

void init_sensors(){
    // qwiic_setup();
    // init_thermocouples();
    // setup_gas();
    // setup_water_flow();
    // humidity_setup();
    // setup_tmp();
    setup_max();
    // current_4_20MA_setup();
    battery_setup();
}

void read_sensors() {
    // thermocouple_loop();
    // loop_gas();
    // loop_ct();
    // humidity_loop();
    // loop_water_flow();
    // loop_tmp();
    loop_max();
    Serial.println(get_temp_c());
    // current_4_20MA_loop();
    battery_loop();
    rssi_loop();
}
