#include <Arduino.h>
#include <SPIFFS.h> 
#include <ArduinoJson.h>
#include "Preferences.h"
#include "wifimanager.h"

Preferences preferences;

// ========================== Setters ==========================

void setWifiSSID(String ssid) {
    preferences.putString("wifi_ssid", ssid);
}

void setWifiPassword(String password) {
    preferences.putString("wifi_password", password);
}

void setMode(String mode) {
    preferences.putString("mode", mode);
}

void setSamplingTime(int sampling) {
    preferences.putInt("sampling_time", sampling);
}

void setIsSleepyMode(bool isSleepy) {
    preferences.putBool("is_sleepy", isSleepy);
}

void setInternalTime(double time) {
    preferences.putDouble("internal_time", time);
}

// ============================ Getters ========================

String getWifiSSID() {
    return preferences.getString("wifi_ssid", "");
}

String getWifiPassword() {
    return preferences.getString("wifi_password", "");
}

String getMode() {
    return preferences.getString("mode", "");
}

int getSamplingTime() {
    return preferences.getInt("sampling_time", 0);
}

bool getIsSleepyMode() {
    return preferences.getBool("is_sleepy", 0);
}

double getInternalTime() {
    return preferences.getDouble("internal_time", 0);
}


// ========================== Set ==========================
void init_preference() {
    preferences.begin("settings", false);
    // preferences.putString("mode", "MENU");
    // preferences.putInt("sampling_time", 60 * 5); // Set defaul 5 min
    // preferences.end();
}

void config_file() {
    init_preference();
}
