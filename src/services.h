#ifndef SERVICES_H
#define SERVICES_H

double get_time_from_server();
String post_sensor(String address, double value, double time);
#endif // SERVICES_H