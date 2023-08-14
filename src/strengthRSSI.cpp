#include <Arduino.h>
#include <WiFi.h>

int bars_rssi = 0;
int rssi = 0;

int get_rssi_strength() {
    return rssi;
}

int get_rssi_strength_bars() {
    return bars_rssi;
}

int rssiToBars(int rssi) {
    if (rssi >= -50) {
        return 4;
    } else if (rssi >= -60) {
        return 3;
    } else if (rssi >= -70) {
        return 2;
    } else if (rssi >= -80) {
        return 1;
    } else {
        return 0;
    }
}

void rssi_loop() {
    rssi = WiFi.RSSI();  // Get RSSI value in dBm
    bars_rssi = rssiToBars(rssi);

    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.print(" dBm, Bars: ");
    Serial.println(bars_rssi);
}
