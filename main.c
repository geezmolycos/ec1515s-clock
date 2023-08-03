
#include "project-defs.h"
#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>
#include <stdlib.h>
#include <pca-hal.h>
#include <math.h>

#include "led-seg.h"
#include "button.h"
#include "sensor.h"
#include "ds1302.h"
#include "buzzer.h"

bool s1_get(){
	P3_0 = 1;
	delay10us(1);
	return P3_0;
}

ButtonConfig s1 = {.get=s1_get};
ButtonEvent s1e;

extern uint8_t led_seg_scan_state;

uint8_t timer_count = 0;
uint8_t row_index = 0;
DS1302_regs regs;
uint16_t t = 0;
INTERRUPT(timer0_isr, TIMER0_INTERRUPT){
	led_seg_auto_scan();
	timer_count++;
	if (timer_count == 43){
		timer_count = 0;
		// ds1302_read_registers(&regs);
		led_seg_row_clear();
		led_seg_col_clear();
		led_seg_off();
		sensor_on();
		uint16_t temp = sensor_temp_celsius(sensor_get_temp());
		uint16_t light = 0;
		light |= (temp >> 6) / 10 << 12;
		light |= (temp >> 6) % 10 << 8;
		light |= (temp & 0x3f) * 10 >> 6;
		seg_set_digit(0, light >> 12 & 0xf);
		seg_set_digit(1, light >> 8 & 0xf);
		seg_set_digit(2, 0x80);
		seg_set_digit(3, light >> 0 & 0xf);
		button_test_event(&s1, &s1e);
		sensor_off();
		led_seg_on();
		putchar(0x87);
		putchar(led_seg_scan_state);
		// putchar(light>>8);
		// putchar(light & 0xff);
		// putchar(0x87);
		// putchar(s1.state);
		// putchar(s1.time);
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
	// buzzer_init();
	ds1302_write_byte(0x8e, 0x00);
	ds1302_write_byte(0x80, 0x00);

	for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x55;
	}

	EA = 1;

	TimerStatus rc = startTimer(
		TIMER0,
		frequencyToSysclkDivisor(2000),
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);

	printf_tiny("Run ok %d!\n", (int)rc);
	while(1);
}
