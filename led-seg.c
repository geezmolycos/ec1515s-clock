
#include "led-seg.h"

void led_seg_init(){
    led_seg_on();
}

void led_seg_on(){
    P1M1 = 0x00;
    P1M0 = 0xff; // pull-push
    P5M1 &= ~(1 << 4 | 1 << 5);
    P5M0 |=  (1 << 4 | 1 << 5);
    P3M1 &= ~(1 << 6 | 1 << 7);
    P3M0 |=  (1 << 6 | 1 << 7);
}

void led_seg_off(){
    P1M1 = 0xff;
    P1M0 = 0x00; // high resistance
    P5M1 |=  (1 << 4 | 1 << 5);
    P5M0 &= ~(1 << 4 | 1 << 5);
    P3M1 |=  (1 << 6 | 1 << 7);
    P3M0 &= ~(1 << 6 | 1 << 7);
}

static void scan_row(uint8_t row_index, uint8_t col_value){
    led_seg_col_clear();
    led_seg_row_clear();
    led_seg_row_set(row_index, true);
    LED_SEG_COL_PORT = col_value;
}

void led_scan_row(uint8_t row_index){
    uint8_t col_value = 0;
    for (uint8_t i=0; i<8; ++i){
        col_value |= led_state(row_index*8 + i) << i;
    }
    col_value = ~col_value; // output 1 will turn led off (common anode)
    scan_row(row_index, col_value);
}

uint8_t seg_digit[SEG_DIGIT_COUNT];
uint8_t seg_colon;

static const uint8_t seg_code[] = {
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71, // 0~F
    0x00,0x73, // none and p
};

void seg_scan_digit(uint8_t digit_index){
    uint8_t col_value = seg_digit[digit_index];
    if (col_value & 0x80){
        col_value &= 0x7f;
    } else {
        col_value = seg_code[col_value];
    }
    col_value |= (seg_colon >> digit_index) << 7;
    col_value = ~col_value; // common anode
    scan_row(LED_ROW_COUNT + digit_index, col_value);
}
