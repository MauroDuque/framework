#include <Arduino.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include "SPIFFS.h"
#include "spiffile.h"
#include "wifimanager.h"

#define EEPROM_SIZE sizeof(int)
#define VERSION 0

JSONVar settings;
JSONVar logs;
JSONVar temperature;
JSONVar time_device;

// const char* base_url = "http://10.0.0.84:3000";
const char* base_url = "http://communication.enerinno.ca";
String log_url = "https://api.enerinno.ca/external/devices/logs";
String firmware_url;

// File paths to save input values permanently
char* ssidPath = "/ssid.txt";
char* passPath = "/pass.txt";
char* ipPath = "/ip.txt";
char* gatewayPath = "/gateway.txt";
char* ssid_local_set = "/ssid_local_set";

char* get_ssid_path() {
  return ssidPath;
}

String get_url_log() {
  return log_url;
}

char* get_pass_path() {
  return passPath;
}

char* get_ip_path() {
  return ipPath;
}

char* get_gateway_path() {
  return gatewayPath;
}

char* get_ssid_local_set() {
  return ssid_local_set;
}

String get_SSID() {
  String ssid = read_file(SPIFFS, ssidPath);
  return ssid;
}

String get_password(){
  String pass = read_file(SPIFFS, passPath);
  return pass;
}

String get_firmware_url() {
  // // http://communication.enerinno.ca/external/devices/firmware
  // const char* firmware_file = "/external/devices/firmware";
  
  // // Combine base URL and firmware file path into a single URL string
  // String url = String(base_url) + firmware_file;
  
  return firmware_url;
}

String get_server_time_url() {
  const char* current_time = "/helpers/current-time";
  
  // Combine base URL and firmware file path into a single URL string
  String url = String(base_url) + current_time;
  
  return url;
}

String get_settings_server_url() {
  // "http://communication.enerinno.ca/external/devices/settings"
  const char* settings = "/external/devices/settings";
  // Combine base URL and firmware file path into a single URL string
  String url = String(base_url) + settings;
  return url;
}

JSONVar get_settings() {
    return settings;
}

JSONVar get_logs() {
  return logs;
}

JSONVar get_time_value() {
  return time_device;
}

int get_firmware_version() {
  return EEPROM.read(VERSION);
}

void get_configuration_server(){
  Serial.println("Load values saved in SPIFFS");

  String ssid;
  String pass;
  String ip;
  String gateway;
  String local_set;

  ssid = read_file(SPIFFS, ssidPath);
  pass = read_file(SPIFFS, passPath);

  ip = read_file(SPIFFS, ipPath);
  gateway = read_file (SPIFFS, gatewayPath);
  local_set = read_file(SPIFFS, ssid_local_set);

  Serial.println(">>>>>>>*<<<<<<<");

  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);
  Serial.println(local_set);
}

void set_settings() {
  EEPROM.begin(EEPROM_SIZE); // Initialize EEPROM

  // File configFile = SPIFFS.open("/config.json", "r");
  // if (!configFile) {
  //   Serial.println("Failed to open config file");
  //   return;
  // }

  // size_t size = configFile.size();
  // std::unique_ptr<char[]> buf(new char[size]);
  // configFile.readBytes(buf.get(), size);

  // DynamicJsonDocument jsonDoc(1024); // Adjust the buffer size as needed
  // deserializeJson(jsonDoc, buf.get());

  // const char* wifi_ssid = jsonDoc["wifi"]["ssid"];
  // const char* wifi_password = jsonDoc["wifi"]["password"];

  // Serial.printf("WiFi SSID: %s\n", wifi_ssid);
  // Serial.printf("WiFi Password: %s\n", wifi_password);

  settings["device"] = get_mac_address();
  settings["softwareVersion"] = get_firmware_version();
  settings["ssid"] = get_SSID();
  settings["password"] = get_password();
}

void set_logs() {
  logs["device"] = get_mac_address();
  logs["log"]["time"] = "56466757656578868";
}

void set_software_version(int version) {
  // Write the number to EEPROM
  EEPROM.write(VERSION, version);
  EEPROM.commit();
}

void set_firmware_url(String url) {
  firmware_url = url;
}

void set_time_value(JSONVar value) {
  time_device["timestamp"] = value;
}

void set_sensor_temperature(double value, String address) {
  temperature["sensor"] = address;
  temperature["value"] = value;
  temperature["date"] = get_time_value()["timestamp"];
  Serial.print("Sensor dat time: ");
  Serial.println(temperature);
}