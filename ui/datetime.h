
#ifndef _UI_DATETIME_H
#define _UI_DATETIME_H

#include "project-defs.h"
#include "main.h"

#define UI_DATETIME_TIME_CONFIG_DS1302_ADDR 0xc0

void time_displayer_init(bool adjust);
void time_displayer_exit(bool adjust);
void time_displayer(bool adjust);

#endif // _UI_DATETIME_H
