#ifndef SPIFFILE_H
#define SPIFFILE_H

#include <SPIFFS.h>

bool init_spiff();
String read_file(fs::FS &fs, const char * path);
void write_file(fs::FS &fs, const char * path, const char * message);
#endif