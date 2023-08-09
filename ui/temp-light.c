
#include "temp-light.h"
#include <stdio.h>

void temp_displayer_init(bool adjust){
    for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x00;
	}
}

void temp_displayer_exit(bool adjust){
    
}

static uint16_t temp_avg = 25 << 6;
static uint16_t light_avg = 0 << 4;
static void temp_light_displayer(bool adjust, bool is_light){
    if (adjust){
        set_adjust(false);
        return;
    }
    IE1 &= ~M_T0IE;
    led_seg_off();
    sensor_on();
    uint16_t temp = is_light ? sensor_get_light() : sensor_get_temp();
    sensor_off();
    led_seg_on();
    IE1 |= M_T0IE;
    temp = is_light ? sensor_light_lux(temp) : sensor_temp_celsius(temp);
    if (!is_light){
        temp_avg = (temp_avg * 7 + temp) / 8;
        seg_set_digit(0, (temp_avg >> 6) / 10 & 0x0f);
        seg_set_digit(1, (temp_avg >> 6) % 10);
        seg_set_digit(2, ((uint8_t)(temp_avg & 0x3f) * 10) >> 6);
        seg_set_digit(3, 0x14);
        seg_set_colon(0x01);
    } else {
        light_avg = light_avg / 8 * 7 + temp / 8;
        if (light_avg >= (1600 << 4)){
            light_avg = (1600 << 4) - 1;
        }
        uint16_t avg = light_avg >> 4;
        seg_set_digit(0, avg / 100 & 0x0f);
        seg_set_digit(1, avg / 10 % 10);
        seg_set_digit(2, avg % 10);
        putchar(0x87);
        putchar((avg%10) >> 8);
        putchar((avg%10));
        seg_set_digit(3, 0x15);
        seg_set_colon(0x00);
    }
    uint16_t avg = (is_light ? light_avg : temp_avg);
    for (uint8_t i=0; i < LED_ROW_COUNT; i++){
        uint8_t bb = 0;
        for (uint8_t j=0; j<8; j++){
            uint8_t id = i*8+j;
            uint16_t threshold = is_light ? 0x140 * id : (uint16_t)0x80 * (id < 50 ? id : (id-60));
            if (threshold - avg >= 0x8000){
                bb |= (1 << j);
            }
        }
        led_seg_state[i] = bb;
	}
}

void temp_displayer(bool adjust){
    temp_light_displayer(adjust, false);
}

void light_displayer(bool adjust){
    temp_light_displayer(adjust, true);
}
