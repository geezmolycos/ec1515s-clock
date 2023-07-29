
#include "project-defs.h"
#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>

#include "led-seg.h"

bool led_state(uint8_t index){
	return index & 1;
}

static bool timer_event = false;
static bool error_event = false;

INTERRUPT(timer0_isr, TIMER0_INTERRUPT){

	if (!timer_event){
		timer_event = true;
	} else {
		error_event = true;
	}
}

uint8_t seg_digit[SEG_DIGIT_COUNT];
uint8_t seg_colon;

void main() {

	serialConsoleInitialise(
		CONSOLE_UART,
		CONSOLE_SPEED, 
		CONSOLE_PIN_CONFIG
	);

	led_seg_init();

	EA = 1;

	TimerStatus rc = startTimer(
		TIMER0,
		frequencyToSysclkDivisor(500), 
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);

	printf_tiny("Run ok %d!\n", (int)rc);

	uint8_t row_index = 0;
	while (true){
		if (error_event){
			error_event = false;
			printf_tiny("can't keep up\n");
		}
		if (timer_event){
			timer_event = false;
			EA = 0;
			led_scan_row(row_index);
			EA = 1;
			row_index++;
			if (row_index == LED_ROW_COUNT){
				row_index = 0;
			}
		}
	}
}
