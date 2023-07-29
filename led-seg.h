
#ifndef _LED_SEG_H
#define _LED_SEG_H

#include "project-defs.h"
#include <gpio-hal.h>

extern bool led_state(uint8_t index);

#define LED_ROW_COUNT 8
#define SEG_DIGIT_COUNT 4
#define LED_SEG_COL_PORT P2
#define LED_SEG_ROW_PIN0 P1_1
#define LED_SEG_ROW_PIN1 P1_0
#define LED_SEG_ROW_PIN2 P1_2
#define LED_SEG_ROW_PIN3 P1_3
#define LED_SEG_ROW_PIN4 P5_4
#define LED_SEG_ROW_PIN5 P5_5
#define LED_SEG_ROW_PIN6 P3_6
#define LED_SEG_ROW_PIN7 P3_7
#define LED_SEG_ROW_PIN8 P1_4
#define LED_SEG_ROW_PIN9 P1_5
#define LED_SEG_ROW_PIN10 P1_6
#define LED_SEG_ROW_PIN11 P1_7

extern uint8_t seg_digit[SEG_DIGIT_COUNT];
extern uint8_t seg_colon;

inline void led_seg_col_clear(){
    LED_SEG_COL_PORT = 0xff;
}
inline void led_seg_row_clear(){
    P1 = P5 = P3_6 = P3_7 = 0;
}

inline void led_seg_row_set(uint8_t row_index, bool value){
    switch (row_index){
        case 0: LED_SEG_ROW_PIN0 = value; break;
        case 1: LED_SEG_ROW_PIN1 = value; break;
        case 2: LED_SEG_ROW_PIN2 = value; break;
        case 3: LED_SEG_ROW_PIN3 = value; break;
        case 4: LED_SEG_ROW_PIN4 = value; break;
        case 5: LED_SEG_ROW_PIN5 = value; break;
        case 6: LED_SEG_ROW_PIN6 = value; break;
        case 7: LED_SEG_ROW_PIN7 = value; break;
        case 8: LED_SEG_ROW_PIN8 = value; break;
        case 9: LED_SEG_ROW_PIN9 = value; break;
        case 10: LED_SEG_ROW_PIN10 = value; break;
        case 11: LED_SEG_ROW_PIN11 = value; break;
        default: break;
    }
}

void led_seg_init();
void led_scan_row(uint8_t row_index);
void seg_scan_digit(uint8_t digit_index);

#endif // _LED_SEG_H
