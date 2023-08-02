#include <SparkFunTMP102.h> // Used to send and recieve specific information from our sensor

TMP102 sensor0;
float tmp_temp;

float get_sensor_temp_102(){
    return tmp_temp;
}

void setup_tmp() {
  if(!sensor0.begin()) {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
  }
  
  Serial.println("Connected to TMP102!");
  delay(100);


  //set Extended Mode.
  //0:12-bit tmp_temp(-55C to +128C) 1:13-bit tmp_temp(-55C to +150C)
  sensor0.setExtendedMode(0);
}
 
void loop_tmp() {
  boolean alertPinState, alertRegisterState;
  
  // Turn sensor on to start tmp_temp measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read tmp_temp data
//   tmp_temp = sensor0.readTempF();
  tmp_temp = sensor0.readTempC();

  sensor0.sleep();

  // Print tmp_temp and alarm state
  Serial.print("tmp_temp: ");
  Serial.print(tmp_temp);
    
  delay(1000);  // Wait 1000ms
}