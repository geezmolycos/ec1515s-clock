
#ifndef _UI_DATETIME_H
#define _UI_DATETIME_H

#include "project-defs.h"
#include "main.h"

#define UI_DATETIME_TIME_CONFIG_DS1302_ADDR 0xc0

void time_displayer_init(bool adjust);
void time_displayer_exit(bool adjust);
void time_displayer(bool adjust);

void date_displayer_init(bool is_adjust);
void date_displayer_exit(bool is_adjust);
void date_displayer(bool adjust);
void year_displayer_init(bool adjust);
void year_displayer(bool adjust);

#endif // _UI_DATETIME_H
