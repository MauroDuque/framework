#include <Arduino.h>
#include <Wire.h>

int qwiicPower = 0; //Thing Plus C 0 is connected to the v-reg that controls the Qwiic power

void qwiic_setup() {
    Wire.begin();
    Wire.setClock(100000);

    pinMode(qwiicPower, OUTPUT);
    digitalWrite(qwiicPower, HIGH);
}

void toggle_off_qwiic() {
    digitalWrite(qwiicPower, LOW);
}

void qwiic_scanning() {
    byte error, address; //variable for error and I2C address
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++ ) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

  
  