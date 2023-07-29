
#include "led-seg7.h"

inline void led_seg7_clear(){
    gpioWrite(&led_seg7_cols, 0xff);
}

static uint8_t led_seg7_on_row_index = 0;

void led_scan_row(uint8_t row_index){
    led_seg7_clear();
    gpioWrite(&led_seg7_rows[led_seg7_on_row_index], 0);
    uint8_t col_value = 0;
    for (uint8_t i=0; i<8; ++i){
        col_value |= led_state(row_index*8 + i) << i;
    }
    col_value = ~col_value; // output 1 will turn led off (common anode)
    gpioWrite(&led_seg7_cols, col_value);
    led_seg7_on_row_index = row_index;
    gpioWrite(&led_seg7_rows[row_index], 1);
}

static __code uint8_t seg7_code[] = {
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71, // 0~F
    0x00,0x73, // none and p
};

void seg7_scan_digit(uint8_t digit_index){
    uint8_t row_index = digit_index + LED_ROW_COUNT;
    led_seg7_clear();
    gpioWrite(&led_seg7_rows[led_seg7_on_row_index], 0);
    uint8_t col_value = seg7_digit[digit_index];
    if (col_value & 0x80){
        col_value &= 0x7f;
    } else {
        col_value = seg7_code[col_value];
    }
    col_value |= (seg7_colon >> digit_index) << 7;
    col_value = ~col_value; // common anode
    gpioWrite(&led_seg7_cols, col_value);
    led_seg7_on_row_index = row_index;
    gpioWrite(&led_seg7_rows[row_index], 1);
}
