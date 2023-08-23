#include <Arduino.h>
#include <SPIFFS.h> 
#include <ArduinoJson.h>
#include "Preferences.h"

Preferences preferences;

String config_file_json = "/config_enerinno.json";
DynamicJsonDocument jsonConfig(1024); // Adjust the buffer size as needed




// -------------------------------------------------------------
//  SPIFF
bool init_spiffs() {
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return false;
    }
    // Serial.println("Success to mount file system");
    return true;
}

File get_file_spiffs(const String& fileName){
    
    File configFileForOpen = SPIFFS.open(fileName, "r");
    if (!configFileForOpen) {
        Serial.println("Failed to open config file");
    }

    // Serial.println("Success to open config file");
    return configFileForOpen;
}


void save_file_spiffs(const String& fileName, DynamicJsonDocument jsonDoc){
    File configFile = SPIFFS.open(fileName, "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return;
    }

    serializeJson(jsonDoc, configFile);
    configFile.close();
}
// End SPIFF
// -------------------------------------------------------------


// ========================== Setters ==========================

void setWifiSSID(String ssid) {
    preferences.putString("wifi_ssid", ssid);
}

void setWifiPassword(String password) {
    // wifi_password = password;
    preferences.putString("wifi_password", password);
}

void setMode(String mode) {
    preferences.putString("mode", mode);
}

void setSamplingTime(int sampling) {
    preferences.putInt("sampling_time", sampling);
}

// ============================ Getters ========================

String getWifiSSID() {
    // return wifi_ssid;
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

void getAllConfig() {
    // setWifiSSID(jsonConfig["wifi"]["ssid"]);
    // setWifiPassword(jsonConfig["wifi"]["password"]);
    // setMode(jsonConfig["config"]["mode"]);
}

// ========================== Set ==========================

void safe_config_file() {
    // jsonConfig["wifi"]["ssid"] = getWifiSSID();
    // jsonConfig["wifi"]["password"] = getWifiPassword();
    // jsonConfig["config"]["mode"] = getMode();
    // save_file_spiffs("/config.json", jsonConfig);
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
//   if(init_spiffs()) {

//     File configFile = get_file_spiffs(config_file_json);
//     if (!configFile) {
//         Serial.println("Failed to open config file");
//         return;
//     }

//     size_t size = configFile.size();
//     std::unique_ptr<char[]> buf(new char[size]);
//     configFile.readBytes(buf.get(), size);

//     deserializeJson(jsonConfig, buf.get());
//     getAllConfig();
//   }
}