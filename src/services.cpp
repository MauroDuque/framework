#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Update.h>
#include "SPIFFS.h"
#include "spiffile.h"
#include "settings.h"
#include "wifimanager.h"
#include "thermocouple.h"
#include "ct.h"
#include "humidity.h"
#include "GAS.h"
#include "water_flow.h"
#include "tmp_102.h"
#include "temp_max.h"
#include "c4to20mA.h"

int version;

static void rebootEspWithReason(String reason) {
  Serial.println(reason);
  delay(1000);
  ESP.restart();
}

void performUpdate(Stream &updateSource, size_t updateSize) {

  if (Update.begin(updateSize)) {      
    size_t written = Update.writeStream(updateSource);
    
    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
    } else {
      Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
    }
    
    if (Update.end()) {
      Serial.println("OTA done!");
      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");

        // --
        set_software_version(version);

        Serial.print("Version updated: ");
        Serial.println(get_firmware_version());

      } else {
        Serial.println("Update not finished? Something went wrong!");
      }
    } else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      Update.printError(Serial);
    }

  } else {
    Serial.println("Not enough space to begin OTA");
    delay(60*60*1000);
  }
}

void updateFromFS(fs::FS &fs) {
  File updateBin = fs.open("/file.bin");
  if (updateBin) {
    if(updateBin.isDirectory()){
      Serial.println("Error, update.bin is not a file");
      updateBin.close();
      return;
    }

    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
      Serial.println("Try to start update");
      performUpdate(updateBin, updateSize);
    } else {
      Serial.println("Error, file is empty");
    }

    updateBin.close();
  
    // whe finished remove the binary from sd card to indicate end of the process
    Serial.println("Removing file...");
    fs.remove("/file.bin");   
    rebootEspWithReason("Rebooting to complete OTA update");   
  } else {
    Serial.println("Could not load update.bin from sd root");
  }
}

void get_time() {
  HTTPClient http_get_time;

  http_get_time.begin(get_server_time_url().c_str());
  
  int httpResponseCodeTime = http_get_time.GET();

  if (httpResponseCodeTime > 0) {
    String payload = http_get_time.getString();
    Serial.print("LOG -> Time request|| HTTP Response code: ");
    Serial.print(httpResponseCodeTime);
    Serial.print(" ");
    Serial.println(payload);

    JSONVar myObject = JSON.parse(payload);
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
    }

    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      Serial.println(value);
      set_time_value(value);
      // return double(value);
    }
  }
  http_get_time.end();
}

void get_bin() {
  HTTPClient http_get_bin;
  http_get_bin.begin(get_firmware_url().c_str());
  // http_get_bin.begin("http://10.0.0.84:3000/firmware");
  // http_get_bin.begin("https://firebasestorage.googleapis.com/v0/b/enerinno-control.appspot.com/o/firmware.bin?alt=media&token=6a74a77e-6185-4ac2-a64d-0d02dc4cf31b");
  
  int httpResponseCodeBin = http_get_bin.GET();
  Serial.println("File.....");
  Serial.println(httpResponseCodeBin);

  if (httpResponseCodeBin == 200) {

    if (SPIFFS.exists("/file.bin")) {
      if (SPIFFS.remove("/file.bin")) {
        Serial.println("File deleted successfully<<<");
      } else {
        Serial.println("File deletion failed<<<<");
      }
    }
    
    File file = SPIFFS.open("/file.bin", FILE_WRITE);
    if (!file) {
      Serial.println("Failed to create file");
      return;
    }

    http_get_bin.writeToStream(&file);

    file.close();
    Serial.println("File downloaded and saved");
  }

  http_get_bin.end();
}

bool update_software() {
  get_bin();
  updateFromFS(SPIFFS);
  return true;
}

void check_version_update(JSONVar myObject) {
  Serial.println("URL BIN SETTINGS---------");
  // Serial.println(myObject["id"]);
  Serial.println(myObject["deviceSettings"]["softwareUrl"]);

  set_firmware_url(myObject["deviceSettings"]["softwareUrl"]);

  version = myObject["deviceSettings"]["softwareVersion"];
  Serial.print("-----VERSION-------> ");
  Serial.println(version);

  if(version != get_firmware_version()) {
    Serial.println("Version different | Update");
    update_software();
  }
}

void post_settings() {
  // Create HTTPClient object
  HTTPClient http;

  // Specify server and resource path
  http.begin(get_settings_server_url().c_str());

  // Set content type header
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request
  int httpResponseCode = http.POST(JSON.stringify(get_settings()));

  // Check for successful response
  if (httpResponseCode > 0) {
    String response = http.getString();
    JSONVar myObject = JSON.parse(response);
    check_version_update(myObject);
  } else {
    Serial.println("HTTP POST request failed");
  }

  // Free resources
  http.end();
}

void post_log() {
  Serial.print("Posting the settings");
  HTTPClient http;

  // Specify server and resource path
  http.begin(get_url_log());

  // Set content type header
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request
  int httpResponseCode = http.POST(JSON.stringify(get_logs()));

  // Check for successful response
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(" ");
    Serial.println("Res: ");
    Serial.println(response);
  } else {
    Serial.println("HTTP POST request failed");
  }

  // Free resources
  http.end();
}

void data_send(String address, double value) {
  Serial.println("Posting data sensor values");
  HTTPClient http;

  // Specify server and resource path
  http.begin("http://communication.enerinno.ca/sensor-events");

  // Set content type header
  http.addHeader("Content-Type", "application/json");
  JSONVar data_to_send;
  // JSONVar timesta = get_time_value();

  double val = value;


  data_to_send["sensor"] = address;
  data_to_send["value"] = val;
  data_to_send["date"] = double(get_time_value()["timestamp"]);
  Serial.println(JSON.stringify(data_to_send));
  // Send HTTP POST request
  int httpResponseCode = http.POST(JSON.stringify(data_to_send));

  // Check for successful response
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(" ");
    Serial.println("Res: ");
    Serial.println(response);
  } else {
    Serial.println("HTTP POST request failed");
  }

  // Free resources
  http.end();
}

void post_sensors_values() {
  // data_send(get_mac() + "&0x60", getSensorTemperature(0));
  // delay(500);
  // data_send(get_mac() + "&0x61", getSensorTemperature(1));
  // delay(500);
  // data_send(get_mac() + "&0x62", getSensorTemperature(2));
  // delay(500);
  // data_send(get_mac() + "&0x63", getSensorTemperature(3));
  // delay(500);
  // data_send(get_mac() + "&0x64", getSensorTemperature(4));
  // delay(500);
  // data_send(get_mac() + "&0x65", getSensorTemperature(5));
  // delay(500);
  // data_send(get_mac() + "&0x66", getSensorTemperature(6));
  // delay(500);
  // data_send(get_mac() + "&0x67", getSensorTemperature(7));
  // delay(500);
  // data_send(get_mac() + "&0x80", get_pulse_gas());
  // delay(500);
  // data_send(get_mac() + "&0x81", get_counter_gas());
  // delay(500);
  // data_send(get_mac() + "&0x40", get_current_1() + 0.1);
  // delay(500);
  // data_send(get_mac() + "&0x41", get_current_2() + 0.1);
  // delay(500);
  // data_send(get_mac() + "&0x42", get_power_1() + 0.1);
  // delay(500);
  // data_send(get_mac() + "&0x43", get_power_2() + 0.1);
  // delay(500);
  // data_send(get_mac() + "&0x44", get_total_power() + 0.1);
  // delay(500);
  // data_send(get_mac() + "&0x90", get_sensor_value_humidity());
  // delay(500);
  // data_send(get_mac() + "&0x91", get_sensor_value_temp());
  // delay(500);
  // data_send(get_mac() + "&0x10", get_liters_per_minute());
  // delay(500);
  // data_send(get_mac() + "&0x11", get_total_volume());
  // delay(500);
  // data_send(get_mac() + "&0x91", get_sensor_temp_102());
  // delay(500);

  
  // data_send(get_mac() + "&0x30", getCurrent() + 0.01);
  // delay(500);

  // data_send(get_mac() + "&0x31", getScalatedMeasure() + 0.01);
  // delay(500);

  data_send(get_mac() + "&0x68", get_temp_c());
  delay(500);
}