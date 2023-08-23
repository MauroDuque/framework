#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <ArduinoJson.h>

void config_file();
extern String getWifiSSID();
extern String getWifiPassword();
extern String getMode();
extern int getSamplingTime();

void setWifiSSID(String ssid);
void setWifiPassword(String password);
void safe_config_file();
void setMode(String mode);
void setSamplingTime(int sampling);
#endif // CONFIG_FILE_H