#include <Arduino.h>
const int buttonPin = 15; // Pin where the button is connected
int previusState = 0;
int counter = 0;
int buttonState = 0;

int get_pulse_gas() {
    return buttonState;
}

int get_counter_gas() {
    return counter;
}

void setup_gas() {
    pinMode(buttonPin, INPUT);
}

void loop_gas() {
    buttonState = digitalRead(buttonPin); // Read the current state of the button
  
    if(buttonState != previusState) {
        previusState = buttonState;
        counter++;
    }
}