#ifndef SERVICES_H
#define SERVICES_H

void get_time();
// void get_settings();
void get_bin();
void post_settings();
void updateFromFS(fs::FS &fs);
bool update_software();
void post_log();

void data_send(String address, double value);
void post_sensors_values();
#endif