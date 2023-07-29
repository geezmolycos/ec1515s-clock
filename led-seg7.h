
#ifndef _LED_SEG7_H
#define _LED_SEG7_H

#include "project-defs.h"
#include <gpio-hal.h>

extern bool led_state(uint8_t index);

#define LED_ROW_COUNT 8
#define SEG7_DIGIT_COUNT 4
extern GpioConfig led_seg7_rows[LED_ROW_COUNT + SEG7_DIGIT_COUNT];
extern GpioConfig led_seg7_cols;

uint8_t seg7_digit[SEG7_DIGIT_COUNT];
uint8_t seg7_colon;

#endif // _LED_SEG7_H
