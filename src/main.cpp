#include <Arduino.h>
#include "menu.h"
#include "config_file.h"
#include "wifimanager.h"
#include "temp_max.h"
#include "services.h"



int PERIOD_CYCLE  = 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

int enable_max = 10;
bool set_up_master = false;


void slave_routine()  {
  Serial.println("Slave Mode");
}

void master_routine() {
  //  -- server --

  if(!set_up_master) {
    delay(5000);
    Serial.println("First loop for setup master mode");
    wifi_connection(getWifiSSID().c_str(), getWifiPassword().c_str());

    pinMode(enable_max, OUTPUT);

    set_up_master = true;
  }

  // if(millis() >= TIME_PERIOD_CYCLE + PERIOD_CYCLE) {
  //   TIME_PERIOD_CYCLE += PERIOD_CYCLE;
    

  // }
  
  // TODO MASTER
  if(millis() >= TIME_PERIOD_CYCLE + (PERIOD_CYCLE * getSamplingTime())) {
    TIME_PERIOD_CYCLE += (PERIOD_CYCLE * getSamplingTime());
    
    digitalWrite(enable_max, HIGH);
    Serial.println("Master Temp[C]: ");
    double temp = get_temp_c();
    Serial.println(temp);

    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Conecting...");
    } else {
      Serial.println("Conected");
      post_sensor(get_mac_address() + "&0x60", temp, get_time_from_server());
    }
  }
}

void setup() {
  Serial.begin(115200);

  config_file();
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
