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

int PERIOD_CYCLE  = 5 * 60 * 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

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

// void mainTask(void* parameter){
//   while(is_internet() == true) {
//     Serial.println("Internet");
//     Serial.println("Getting time....");
//     get_time();
//     delay(500);
//     Serial.println("Posting data sensors....");
//     post_sensors_values();
//     delay(500);
//     Serial.println("Posting settings....");
//     post_settings();
//     post_log();

//     vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay 1 second
//   }
//   vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay 1 second
// }

void setup() {
  Serial.begin(115200);

  init_spiff();
  delay(1000);
  set_settings();
  delay(1000);
  
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
  // init_sensors();
  // delay(1000);

  ota_setup();

  // xTaskCreate(mainTask, "MainTask", 4096, NULL, 1, &mainTaskHandle);
  // Start FreeRTOS scheduler
  // vTaskStartScheduler();
}

void loop() {
  ota_loop();

  if(millis() >= TIME_PERIOD_CYCLE + PERIOD_CYCLE) {
    TIME_PERIOD_CYCLE += PERIOD_CYCLE;
    if(is_internet() == true) {
      // read_sensors();
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

  Serial.print("Strength");
  Serial.println(get_rssi_strength_bars());
  delay(500);
}
