
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

uint8_t last_second = 0xff;
uint8_t colon_count_slow = 0;
void time_displayer(){
	ds1302_read_registers(&regs);
	uint8_t hour = ds1302_regs_get_bcd_hour_24(&regs);
	uint8_t minute = ds1302_regs_get_bcd_minute(&regs);
	uint8_t second = ds1302_regs_get_bcd_second(&regs);
	seg_set_digit(0, (hour & 0xf0) >> 4);
	seg_set_digit(1, hour & 0x0f);
	seg_set_digit(2, (minute & 0xf0) >> 4);
	seg_set_digit(3, minute & 0x0f);
	if (second != last_second){
		colon_count_slow = count_slow;
	}
	last_second = second;
	bool colon = true;
	if ((uint8_t)(count_slow - colon_count_slow) > 25){
		colon = false;
	}
	seg_set_colon(colon ? 0x03 : 0x00);
	for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x00;
	}
	uint8_t place = ((second & 0xf0) >> 4) * 10 + (second & 0x0f);
	uint8_t byte_i = place / 8, bit_i = place % 8;
	// putchar(0x88);
	// putchar(count_slow);
	// putchar(colon_count_slow);
	led_seg_state[byte_i] = 1 << bit_i;
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
		time_displayer();
	}
}
