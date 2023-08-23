#ifndef SAVE_DATA_H
#define SAVE_DATA_H

void addDataEntry(const char* sensor, unsigned long date, int value);
void save_date_setup();
void saveJSONToFile();
void printDataEntries();
void addDataEntry(String sensor, double date, double value);
JsonArray& getDataArray();
#endif // SAVE_DATA_H