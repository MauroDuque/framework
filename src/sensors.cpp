#include <Arduino.h>
#include "thermocouple.h"
#include "qwiic.h"
#include "settings.h"
#include "ct.h"
#include "humidity.h"
#include "GAS.h"
#include "water_flow.h"
#include "tmp_102.h"

void init_sensors(){
    qwiic_setup();
    //init_thermocouples();
    setup_gas();
    // setup_water_flow();
    humidity_setup();
    //setup_tmp();
}

void read_sensors() {
    //thermocouple_loop();
    loop_gas();
    //loop_ct();
    // loop_gas();
    humidity_loop();
    // loop_water_flow();
    //loop_tmp();
}
 

// {
//   "code": "7C:DF:A1:55:B5:C6",
//   "name": "Fieldgate",
//   "description": "Fieldgate",
//   "sensors": [
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x60",
//       "name": "Temperature 1"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x61",
//       "name": "Temperature 2"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x62",
//       "name": "Temperature 3"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x63",
//       "name": "Temperature 4"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x64",
//       "name": "Temperature 5"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x65",
//       "name": "Temperature 6"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x66",
//       "name": "Temperature 7"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x67",
//       "name": "Temperature 8"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x10",
//       "name": "GAS_COUNTER"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x11",
//       "name": "GAS_COUNTER"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x80",
//       "name": "Pulse GAS"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x40",
//       "name": "Current 0"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x41",
//       "name": "Current 1"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x90",
//       "name": "Current 0"
//     },
//     {
//       "code": "7C:DF:A1:55:B5:C6&0x91",
//       "name": "Current 1"
//     }
//   ]
// }