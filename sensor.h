
#ifndef _SENSOR_H
#define _SENSOR_H

#include "project-defs.h"

#define SENSOR_TEMP_CHANNEL 4
#define SENSOR_LIGHT_CHANNEL 5

void sensor_init();
void sensor_on();
void sensor_off();
uint16_t sensor_get_light();
uint16_t sensor_get_temp();

#endif // _SENSOR_H
