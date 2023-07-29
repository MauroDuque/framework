#include <Arduino.h>
#include "SPIFFS.h"
#include "wifimanager.h"
#include "services.h"
#include "spiffile.h"
#include "settings.h"
#include "server.h"
#include "sensors.h"


#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <ElegantOTA.h>

WebServer server_ota(85);

int PERIOD_CHECK_CONNECTION  = 1 * 60 * 1000;
unsigned long TIME_PERIOD_CHECK_CONNECTION = 0;

int PERIOD_CYCLE  = 0.3 * 60 * 1000;
unsigned long TIME_PERIOD_CYCLE = 0;

void setup() {
  Serial.begin(115200);

  init_spiff();
  delay(1000);
  set_settings();
  delay(1000);
  wifi_setup();
  delay(1000);
  get_configuration_server();
  delay(1000);
  init_server();
  delay(1000);
  set_logs();
  delay(1000);
  init_sensors();
  delay(1000);

  server_ota.on("/", []() {
    server_ota.send(200, "text/plain", "Hi! I am ESP8266.");
  });

  ElegantOTA.begin(&server_ota);    // Start ElegantOTA
  server_ota.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server_ota.handleClient();
  if(millis() >= TIME_PERIOD_CHECK_CONNECTION + PERIOD_CHECK_CONNECTION) {
    TIME_PERIOD_CHECK_CONNECTION += PERIOD_CHECK_CONNECTION;
    check_wifi_connection();
  }

  

  if(millis() >= TIME_PERIOD_CYCLE + PERIOD_CYCLE) {
    TIME_PERIOD_CYCLE += PERIOD_CYCLE;
    read_sensors();
    if(is_internet() == true) {
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
}
