
#ifndef _UI_TEMP_LIGHT_H
#define _UI_TEMP_LIGHT_H

#include "project-defs.h"
#include "main.h"

#define UI_AUTO_BRIGHTNESS_DS1302_ADDR 0xc1

void temp_displayer(bool adjust);
void light_displayer(bool adjust);
void temp_displayer_init(bool adjust);
void temp_displayer_exit(bool adjust);

#endif // _UI_TEMP_LIGHT_H
