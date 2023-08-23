#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

double get_time_from_server() {
  HTTPClient http_get_time;
  http_get_time.begin("http://communication.enerinno.ca/helpers/current-time");

  int httpResponseCodeTime = http_get_time.GET();
  double timestamp = 0.0;  // Initialize with a default value

  if (httpResponseCodeTime > 0) {
    String payload = http_get_time.getString();
    Serial.print("LOG -> Time request || HTTP Response code: ");
    Serial.print(httpResponseCodeTime);
    Serial.print(" ");
    Serial.println(payload);

    DynamicJsonDocument jsonDoc(1024);
    DeserializationError error = deserializeJson(jsonDoc, payload);

    if (!error) {
      timestamp = jsonDoc["timestamp"].as<double>();
    } else {
      Serial.println("Failed to parse JSON response");
    }
  } else {
    Serial.print("HTTP GET request for time failed, error: ");
    Serial.println(http_get_time.errorToString(httpResponseCodeTime).c_str());
  }

  http_get_time.end();
  return timestamp;
}

String post_sensor(String address, double value, double time) {
    Serial.print("Sensor: ");
    Serial.print(address);

    Serial.print(" Value: ");
    Serial.print(value);

    Serial.print(" date: ");
    Serial.println(time);

    String responseMessage;
    Serial.println("Connected to WiFi");

    DynamicJsonDocument jsonDataSend(1024);
    jsonDataSend["sensor"] = address;
    jsonDataSend["value"] = value;
    jsonDataSend["date"] = time;

    // Serialize the JSON object to a string
    String jsonString;
    serializeJson(jsonDataSend, jsonString);

    // Make an HTTP POST request
    HTTPClient http;
    http.begin("http://communication.enerinno.ca/sensor-events"); // Replace with your API endpoint
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
        responseMessage = http.getString();
        Serial.println(responseMessage);
    } else {
        Serial.printf("HTTP POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    return responseMessage;
}