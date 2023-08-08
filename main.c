
#include "main.h"

#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>
#include <stdlib.h>
#include <pca-hal.h>
#include <math.h>

#include "datetime.h"

bool s1_get(){
	P3_1 = 1;
	delay10us(1);
	return P3_1;
}
ButtonConfig s1 = {.get=s1_get};
ButtonEvent s1e;

bool s2_get(){
	P3_0 = 1;
	delay10us(1);
	return P3_0;
}
ButtonConfig s2 = {.get=s2_get};
ButtonEvent s2e;

DS1302_regs regs;

uint16_t before_t0;
uint16_t after_t0;
uint8_t count_fast = 0;
uint8_t count_slow = 0;
INTERRUPT(timer0_isr, TIMER0_INTERRUPT){
	before_t0 = T0;
	led_seg_auto_scan();
	count_fast++;
	if (count_fast == 160){
		count_fast = 0;
		count_slow++;
	}
	after_t0 = T0;
}

void (*const displayer[4])(bool) = {time_displayer};
void (*const displayer_init[4])(bool) = {time_displayer_init};
void (*const displayer_exit[4])(bool) = {time_displayer_exit};
uint8_t current_displayer = 0;
uint8_t is_adjust = false;

void main() {

	serialConsoleInitialise(
		CONSOLE_UART,
		CONSOLE_SPEED, 
		CONSOLE_PIN_CONFIG
	);

	led_seg_init();
	sensor_init();
	buzzer_init();
	// buzzer_off();
	if (ds1302_read_byte(0x8e)){ // disable write protection
		ds1302_write_byte(0x8e, 0x00);
	}
	uint8_t s = ds1302_read_byte(0x81);
	if (!(s & 0x80)){ // enable clock
		ds1302_write_byte(0x80, s & 0x7f);
	}

	for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x55;
	}

	// fast timer for matrix scanning
	TimerStatus rc0 = startTimer(
		TIMER0,
		frequencyToSysclkDivisor(8000),
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);
	// IP1L |= 0x02; // set T0 as high priority

	uint16_t full_t0 = T0;

	EA = 1;

	printf_tiny("Run ok %d!\n", (int)rc0);
	
	while(1){
		delay1ms(18);
		// putchar(0x87);
		// putchar(full_t0 >> 8);
		// putchar(full_t0 >> 0);
		// putchar((before_t0) >> 8);
		// putchar((before_t0) >> 0);
		// putchar((after_t0) >> 8);
		// putchar((after_t0) >> 0);
		ds1302_read_registers(&regs);
		button_test_event(&s1, &s1e);
		button_test_event(&s2, &s2e);
		// putchar(0x87);
		// putchar(s2.state);
		// putchar(s2.series);
		// putchar(s2e.type);
		displayer[current_displayer](is_adjust);
		if (
			(s1e.type == BUTTONEVENT_DOWN || s2e.type == BUTTONEVENT_DOWN)
			&& (
				s1.state & M_BUTTON_STATE_ISDOWN
				&& s2.state & M_BUTTON_STATE_ISDOWN
			)
		){
			displayer_exit[current_displayer](is_adjust);
			is_adjust = !is_adjust;
			displayer_init[current_displayer](is_adjust);
		}
	}
}
