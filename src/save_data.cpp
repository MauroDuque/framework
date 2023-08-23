#include <Arduino.h>
#include <SPIFFS.h>

const int MAX_ROWS = 10;

struct LogEntry {
  char sensor[20];
  double time;
  double value;
};

int logSize = 0;


void printLogEntries() {
  Serial.println("Log Entries:");
  File dataFile = SPIFFS.open("/log.txt", FILE_READ);
  if (dataFile) {
    while (dataFile.available()) {
      LogEntry entry;
      dataFile.read((uint8_t*)&entry, sizeof(LogEntry));
      Serial.print("Sensor: ");
      Serial.print(entry.sensor);
      Serial.print(", Time: ");
      Serial.print(entry.time);
      Serial.print(", Value: ");
      Serial.println(entry.value);
    }
    dataFile.close();
  } else {
    Serial.println("Error opening log file.");
  }
}

void saveLogSize() {
  File sizeFile = SPIFFS.open("/log_size.txt", FILE_WRITE);
  if (sizeFile) {
    sizeFile.write((uint8_t*)&logSize, sizeof(int));
    sizeFile.close();
  } else {
    Serial.println("Error opening log size file.");
  }
}

void loadLogSize() {
  File sizeFile = SPIFFS.open("/log_size.txt", FILE_READ);
  if (sizeFile) {
    sizeFile.read((uint8_t*)&logSize, sizeof(int));
    sizeFile.close();
  } else {
    Serial.println("Error opening log size file.");
  }
}


void pushLogEntry(String sensor, double time, double value) {
  if (logSize >= MAX_ROWS) {
    Serial.println("Log is full. Cannot push more entries.");
    return;
  }

  LogEntry entry;
  sensor.toCharArray(entry.sensor, 20);
  entry.time = time;
  entry.value = value;

  File dataFile = SPIFFS.open("/log.txt", FILE_APPEND);
  if (dataFile) {
    dataFile.write((uint8_t*)&entry, sizeof(LogEntry));
    dataFile.close();
    logSize++;
    saveLogSize();
    Serial.println("Entry pushed.");
  } else {
    Serial.println("Error opening log file.");
  }
}

void save_data_setup(){
  if (!SPIFFS.begin()) {
      Serial.println("SPIFFS initialization failed!");
      return;
    }

    loadLogSize();
}
