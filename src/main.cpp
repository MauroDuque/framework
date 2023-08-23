#include <Arduino.h>
#include "menu.h"
#include "config_file.h"
#include "wifimanager.h"
#include "temp_max.h"
#include "services.h"
#include "save_data.h"
#include <RTClib.h> // Include the RTC library

RTC_DS3231 rtc; // Create an instance of the RTC object


int PERIOD_CYCLE  = 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

int enable_max = 10;
bool set_up_master = false;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void saving_data() {
  digitalWrite(enable_max, HIGH);
  Serial.println("Master Temp[C]: ");
  double temp = get_temp_c();
  Serial.println(temp);

  addDataEntry(get_mac_address() + "&0x60", temp, get_time_from_server());
  

  // if(WiFi.status() != WL_CONNECTED) {
  //   Serial.println("Conecting...");
  // } else {
  //   Serial.println("Conected");
  //   // post_sensor(get_mac_address() + "&0x60", temp, getInternalTime());
  //   addDataEntry(get_mac_address() + "&0x60", temp, getInternalTime());
  // }
}

void handle_sleepy_mode() {
  esp_sleep_enable_timer_wakeup(getSamplingTime() * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(getSamplingTime()) + " Seconds");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting..");
  }
  saving_data();
  printDataEntries();

  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

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

    if(getIsSleepyMode()) {
      handle_sleepy_mode();
    }
  }
  
  // TODO MASTER
  if(millis() >= TIME_PERIOD_CYCLE + (PERIOD_CYCLE * getSamplingTime())) {
    TIME_PERIOD_CYCLE += (PERIOD_CYCLE * getSamplingTime());
    saving_data();
    // printDataEntries();
  }
}

void setup() {
  Serial.begin(115200);
  print_wakeup_reason();
  config_file();
  save_date_setup();
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
