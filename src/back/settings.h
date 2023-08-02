#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino_JSON.h>

String get_firmware_url();
String get_server_time_url();
String get_settings_server_url();
JSONVar get_settings();

void set_settings();
void set_mac(String macaddress);
void set_software_version(int version);
char* get_ssid_path();
char* get_pass_path();
char* get_ip_path();
char* get_gateway_path();
char* get_ssid_local_set();
String get_SSID();
String get_password();

void get_configuration_server();
int get_firmware_version();
String get_url_log();
JSONVar get_logs();
void set_logs();
void set_firmware_url(String url);
void set_time_value(JSONVar value);
void set_sensor_temperature(double value, String address);
JSONVar get_time_value();
#endif // SETTINGS_H