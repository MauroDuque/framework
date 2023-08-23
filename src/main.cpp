#include <Arduino.h>
#include "menu.h"
#include "config_file.h"
#include "wifimanager.h"
#include "temp_max.h"


int PERIOD_CYCLE  = 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

int enable_max = 10;

void slave_routine()  {
  Serial.println("Slave Mode");
}

void master_routine() {
  //  -- server --
  // TODO MASTER
  if(millis() >= TIME_PERIOD_CYCLE + (PERIOD_CYCLE * getSamplingTime())) {
    TIME_PERIOD_CYCLE += (PERIOD_CYCLE * getSamplingTime());
    digitalWrite(enable_max, HIGH);
    Serial.println("Master Temp[C]: ");
    Serial.println(get_temp_c());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(enable_max, OUTPUT);

  config_file();

  
  // -- Read Config file --
  // Serial.println("Config..");
  // config_file();
  // Create storage file
  // Create time file
}

void loop() {
  String mode = getMode();
  
  if (strcmp(mode.c_str(), "MENU") == 0) {
    // -- OTA --
    // -- WIFI --
    // -- serve --

    // -- Menu --
  
    String* ssids = get_network_ssids();
    menu(ssids);

  } 
  else if (strcmp(mode.c_str(), "MASTER") == 0) {
    // -- OTA --
    // -- WIFI --
    // -- serve --

    // -- Master mode --
    master_routine();
  }
  else if (strcmp(mode.c_str(), "SLAVE") == 0) {
    // IF SLEEPY MODE is false -> OTA, WIFI, serve

    // -- Slave mode --
    slave_routine();
  }
  else {
    Serial.println("Unknown mode");
  }
}
