
#ifndef _MAIN_H
#define _MAIN_H

#include "project-defs.h"

#include "led-seg.h"
#include "button.h"
#include "sensor.h"
#include "ds1302.h"
#include "buzzer.h"

#define DISPLAYER_AMOUNT 3

extern ButtonConfig s1;
extern ButtonEvent s1e;

extern ButtonConfig s2;
extern ButtonEvent s2e;

extern DS1302_regs regs;

extern uint16_t before_t0;
extern uint16_t after_t0;

extern uint8_t count_fast;
extern uint8_t count_slow;

#endif // _MAIN_H
