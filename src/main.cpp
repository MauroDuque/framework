#include <Arduino.h>
#include "SPIFFS.h"
#include "wifimanager.h"
#include "services.h"
#include "spiffile.h"
#include "settings.h"
#include "server.h"
#include "sensors.h"
#include "temp_max.h"

#include <WiFi.h>
#include <WiFiClient.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ota_setup.h"
#include "ct.h"

TaskHandle_t wifiTaskHandle = NULL;
// TaskHandle_t mainTaskHandle = NULL;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5 * 60        /* Time ESP32 will go to sleep (in seconds) */

bool is_sleepy = false;

int PERIOD_CYCLE  = 0.2 * 60 * 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

void core() {
  if(WiFi.status() == WL_CONNECTED) {
    read_sensors();
    Serial.println("Internet");
    Serial.println("Getting time....");
    get_time();
    delay(500);
    Serial.println("Posting data sensors....");
    post_sensors_values();
    delay(500);
    Serial.println("Posting settings....");
    post_settings();
    post_log();
  } else {
    Serial.println("No internet");
  }
}

void sleeping() {
    /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();
}

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

// Function to connect to WiFi network
void connectToWiFi(void* parameter) {
  Serial.println("Connecting to WiFi...");
  wifi_connection(get_SSID().c_str(), get_password().c_str());
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay 1 second
  }
  
  Serial.println("\nConnected to WiFi");
  vTaskDelete(NULL); // Delete the task
}

void awake_setup(){
  // Create the WiFi task
  xTaskCreate(
    connectToWiFi,          // Function to run
    "WiFiTask",             // Name of the task
    4096,                   // Stack size (adjust as needed)
    NULL,                   // Task parameters
    1,                      // Task priority
    &wifiTaskHandle         // Task handle
  );

  // --------------------------------------
  delay(1000);
  get_configuration_server();
  delay(1000);
  init_server();
  delay(1000);
  set_logs();
  delay(1000);
  init_sensors();
  delay(1000);

  ota_setup();

  Serial.print("MAC: ");
  Serial.println(get_mac_address());
  Serial.println("This will never be printed");
}

void sleepy_setup() {
  print_wakeup_reason();

  Serial.println("Connecting to WiFi...");
  wifi_connection(get_SSID().c_str(), get_password().c_str());
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  
  Serial.println("\nConnected to WiFi");
  core();
  sleeping();
}
void setup() {
  Serial.begin(115200);
  init_spiff();
  delay(1000);
  set_settings();
  delay(1000);

  if(is_sleepy) sleepy_setup();
  awake_setup();
}

void loop() {
  ota_loop();

  if(millis() >= TIME_PERIOD_CYCLE + PERIOD_CYCLE) {
    TIME_PERIOD_CYCLE += PERIOD_CYCLE;
    core();
  }
}
