#include <ArduinoJson.h>
#include <SPIFFS.h>

const int maxEntries = 5000;  // Maximum number of data entries
const size_t capacity = JSON_OBJECT_SIZE(maxEntries);

StaticJsonDocument<capacity> doc;
JsonArray dataArray = doc.to<JsonArray>();

bool readJSONFromFile() {
  File file = SPIFFS.open("/data.json", "r");
  if (!file) {
    Serial.println("Failed to open file");
    return false;
  }

  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse JSON");
    return false;
  }

  return true;
}

void printDataEntries() {
  if (!readJSONFromFile()) {
    Serial.println("Error reading JSON file");
    return;
  }

  for (JsonVariant value : dataArray) {
    String sensor = value["sensor"];
    double date = value["date"];
    double dataValue = value["value"];

    Serial.print("Sensor: ");
    Serial.println(sensor);
    Serial.print("Date: ");
    Serial.println(date);
    Serial.print("Value: ");
    Serial.println(dataValue);
    Serial.println();
  }
}

JsonArray& getDataArray() {
  return dataArray;
}

void saveJSONToFile() {
  File file = SPIFFS.open("/data.json", "w");
  if (!file) {
    Serial.println("Failed to create file");
    return;
  }

  serializeJson(doc, file);
  file.close();
  Serial.println("JSON data saved to file");
}

void addDataEntry(String sensor, double date, double value) {
  if (dataArray.size() >= maxEntries) {
    // Create a new array to replace the existing one
    JsonArray newArray = doc.createNestedArray();
    dataArray = newArray;
  }

  JsonObject dataEntry = dataArray.createNestedObject();
  dataEntry["date"] = date;
  dataEntry["value"] = value;
  dataEntry["sensor"] = sensor;

  saveJSONToFile();
}

void save_date_setup() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
}