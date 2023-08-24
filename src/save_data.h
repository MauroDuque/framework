#ifndef SAVE_DATA_H
#define SAVE_DATA_H

void save_data_setup();
void pushLogEntry(String sensor, double time, double value);
void printLogEntries();
void clearLogEntries();
#endif // SAVE_DATA_H