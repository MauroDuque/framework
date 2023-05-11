#include "WiFi.h"
#include "settings.h"

int counter_connection = 0;

const char *ssid_default = "Enerinno";

String macaddress = WiFi.macAddress();
const char* stringmac = macaddress.c_str();
const char *ssid_mac = stringmac;
char* soft_ap_password = NULL;

char soft_ap_ssid[30];

int counter_attempts = 60;

String get_mac() {
    return macaddress;
}

void wifi_connection() {
    strcpy(soft_ap_ssid, ssid_default);
    strcat(soft_ap_ssid, ssid_mac);

    Serial.println("Setting AP (Access Point & Station)");
    WiFi.mode(WIFI_AP_STA);
    // WiFi.mode(WIFI_STA);

    WiFi.softAP(soft_ap_ssid, soft_ap_password);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP()); // Print the IP address of the Access Point
  
    WiFi.begin(get_SSID().c_str(), get_password().c_str());

    Serial.println("NetWork: ");
    Serial.println(soft_ap_ssid);
}

bool is_internet() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    return true;
}

void check_wifi_connection() {
    if (!is_internet()) {
        Serial.println("Connecting...");
        counter_connection++;
        if(counter_connection >= counter_attempts) ESP.restart();
    } else {
        Serial.println("Connected to WiFi");
    }
}

void wifi_setup() {
    wifi_connection();
}