
#ifndef _DS1302_H
#define _DS1302_H

#include "project-defs.h"

#define DS1302_PIN_CLK P3_2
#define DS1302_PIN_IO P3_3
#define DS1302_PIN_CE P3_4

void ds1302_reset();
void ds1302_write_byte(uint8_t cmd, uint8_t byte);
uint8_t ds1302_read_byte(uint8_t cmd);


#endif // _DS1302_H
