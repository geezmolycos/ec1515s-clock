
#include "project-defs.h"
#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>
#include <stdlib.h>
#include <pca-hal.h>

#include "led-seg.h"
#include "button.h"
#include "sensor.h"
#include "ds1302.h"
#include "buzzer.h"

// uint16_t light = 0x55aa;

bool led_state(uint8_t index){
	// if (index < 16){
	// 	return (light >> index) & 1;
	// }
	return index & 1;
}

bool s1_get(){
	P3_0 = 1;
	delay10us(1);
	return P3_0;
}

ButtonConfig s1 = {.get=s1_get};
ButtonEvent s1e;

uint8_t timer_count = 0;
uint8_t row_index = 0;
DS1302_regs regs;
uint16_t t = 0;
INTERRUPT(timer2_isr, TIMER2_INTERRUPT){
	pcaSetDutyCycle(BUZZER_PCA_CHANNEL, (((t*4)*(42&(t*4)>>10) & 0xff) >> 1) + 64);
	t++;
	timer_count++;
	if (timer_count == 43){
		timer_count = 0;
		ds1302_read_registers(&regs);
		uint16_t light = ds1302_regs_get_bcd_minute(&regs) << 8 | ds1302_regs_get_bcd_second(&regs);
		seg_digit[0] = light >> 12 & 0xf;
		seg_digit[1] = light >> 8 & 0xf;
		seg_digit[2] = light >> 4 & 0xf;
		seg_digit[3] = light >> 0 & 0xf;
		// button_test_event(&s1, &s1e);
		// led_seg_row_clear();
		// led_seg_col_clear();
		// led_seg_off();
		// sensor_on();
		// uint16_t light = sensor_get_temp();
		// seg_digit[0] = light >> 12 & 0xf;
		// seg_digit[1] = light >> 8 & 0xf;
		// seg_digit[2] = light >> 4 & 0xf;
		// seg_digit[3] = light >> 0 & 0xf;
		// sensor_off();
		// led_seg_on();
		// putchar(0x87);
		// putchar(light>>8);
		// putchar(light & 0xff);
		// putchar(0x87);
		// putchar(s1.state);
		// putchar(s1.time);
	}
	if (row_index < LED_ROW_COUNT){
		led_scan_row(row_index);
	} else {
		uint8_t digit_index = row_index - LED_ROW_COUNT;
		while (digit_index > SEG_DIGIT_COUNT){
			digit_index -= SEG_DIGIT_COUNT;
		}
		seg_scan_digit(digit_index);
	}
	// row_index = rand() % (LED_ROW_COUNT + SEG_DIGIT_COUNT);
	row_index++;
	if (row_index >= LED_ROW_COUNT + 4*SEG_DIGIT_COUNT){
		row_index = 0;
	}
}

void main() {

	serialConsoleInitialise(
		CONSOLE_UART,
		CONSOLE_SPEED, 
		CONSOLE_PIN_CONFIG
	);

	led_seg_init();
	sensor_init();
	buzzer_init();
	ds1302_write_byte(0x8e, 0x00);
	ds1302_write_byte(0x80, 0x00);

	seg_digit[0] = 1;
	seg_colon = 0x01;

	EA = 1;

	TimerStatus rc = startTimer(
		TIMER2,
		frequencyToSysclkDivisor(2000), 
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);

	printf_tiny("Run ok %d!\n", (int)rc);
	while(1);
}
