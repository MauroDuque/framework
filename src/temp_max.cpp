#include <SPI.h>
#include "max6675.h"

// Pin definitions for MAX6675ISA+T
const int MAX6675_CS_PIN = 34; // chip select
const int MAX6675_SO_PIN = 37; // data out
const int MAX6675_SCK_PIN = 36; // clock
MAX6675 thermocouple(MAX6675_SCK_PIN, MAX6675_CS_PIN, MAX6675_SO_PIN);

void setup_max() {
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop_max() {
  // basic readout test, just print the current temp
  
  Serial.print("C = "); 
  Serial.println(thermocouple.readCelsius());
  Serial.print("F = ");
  Serial.println(thermocouple.readFahrenheit());
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
//   delay(1000);
}