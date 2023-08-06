
#include "led-seg.h"

uint8_t led_seg_state[LED_ROW_COUNT + SEG_DIGIT_COUNT];

void led_seg_init(){
    led_seg_on();
}

void led_seg_on(){
    P1M1 = 0x00;
    P1M0 = 0xff; // pull-push
    P2M1 = 0xff;
    P2M0 = 0xff; // open-drain
    P5M1 &= ~(1 << 4 | 1 << 5);
    P5M0 |=  (1 << 4 | 1 << 5);
    P3M1 &= ~(1 << 6 | 1 << 7);
    P3M0 |=  (1 << 6 | 1 << 7);
}

void led_seg_off(){
    P1M1 = 0xff;
    P1M0 = 0x00; // high resistance
    P2M1 = 0xff;
    P2M0 = 0x00;
    P5M1 |=  (1 << 4 | 1 << 5);
    P5M0 &= ~(1 << 4 | 1 << 5);
    P3M1 |=  (1 << 6 | 1 << 7);
    P3M0 &= ~(1 << 6 | 1 << 7);
}

void led_seg_scan_row(uint8_t row_index){
    uint8_t col_value = ~led_seg_state[row_index]; // output 1 will turn led off (common anode)
    led_seg_col_clear();
    led_seg_row_clear();
    led_seg_row_set(row_index, true);
    LED_SEG_COL_PORT = col_value;
}

static const uint8_t seg_code[] = {
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71, // 0~F
    0x00,0x73, // none and p
};

void seg_set_digit(uint8_t index, uint8_t value){
    if (value & 0x80){
        value &= 0x7f; // keep original
    } else {
        value = seg_code[value];
    }
    led_seg_state[index + LED_ROW_COUNT] = value;
}

void seg_set_colon(uint8_t colon){
    led_seg_state[LED_ROW_COUNT] |= (colon & 0) << 7;
    led_seg_state[LED_ROW_COUNT + 1] |= (colon & 1) << 7;
}

LEDSegScanState led_seg_scan_state = LED_SEG_SCAN_NONE;
uint8_t led_seg_scan_time[LED_SEG_SCAN_COUNT] = {0, 1, 4};
uint8_t led_seg_scan_round = 0;
uint8_t led_seg_scan_row_index = 0;

void led_seg_auto_scan(){
    switch (led_seg_scan_state){
        case LED_SEG_SCAN_NONE:
            led_seg_col_clear();
            led_seg_row_clear();
            led_seg_scan_round++;
            if (led_seg_scan_round == led_seg_scan_time[LED_SEG_SCAN_NONE]){
                led_seg_scan_round = 0;
                led_seg_scan_row_index = 0;
                led_seg_scan_state = LED_SEG_SCAN_LED;
            }
            break;
        case LED_SEG_SCAN_LED:
            led_seg_scan_row(led_seg_scan_row_index);
            led_seg_scan_row_index++;
            if (led_seg_scan_row_index == LED_ROW_COUNT){
                led_seg_scan_row_index = 0;
                led_seg_scan_round++;
            }
            if (led_seg_scan_round == led_seg_scan_time[LED_SEG_SCAN_LED]){
                led_seg_scan_round = 0;
                led_seg_scan_row_index = LED_ROW_COUNT;
                led_seg_scan_state = LED_SEG_SCAN_SEG;
            }
            break;
        case LED_SEG_SCAN_SEG:
            led_seg_scan_row(led_seg_scan_row_index);
            led_seg_scan_row_index++;
            if (led_seg_scan_row_index == LED_ROW_COUNT + SEG_DIGIT_COUNT){
                led_seg_scan_row_index = LED_ROW_COUNT;
                led_seg_scan_round++;
            }
            if (led_seg_scan_round == led_seg_scan_time[LED_SEG_SCAN_SEG]){
                led_seg_scan_round = 0;
                led_seg_scan_row_index = 0;
                led_seg_scan_state = LED_SEG_SCAN_NONE;
            }
            break;
        default:
            led_seg_scan_state = 0;
    }
    while (led_seg_scan_time[led_seg_scan_state] == 0){
        led_seg_scan_state++;
        if (led_seg_scan_state >= LED_SEG_SCAN_COUNT){
            led_seg_scan_state = 0;
        }
    }
}
