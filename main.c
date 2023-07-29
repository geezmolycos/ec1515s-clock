
#include "project-defs.h"
#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>

#include "led-seg7.h"

bool led_state(uint8_t index){
	return index & 1;
}

static uint16_t timer_count = 0;
static bool timer_event = false;

INTERRUPT(timer0_isr, TIMER0_INTERRUPT){
	timer_count++;
	if (timer_count == 5){
		timer_count = 0;
		timer_event = true;
	}
}

GpioConfig led_seg7_rows[LED_ROW_COUNT + SEG7_DIGIT_COUNT] = {
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN1, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN0, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN2, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN3, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT5, GPIO_PIN4, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT5, GPIO_PIN5, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT3, GPIO_PIN6, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT3, GPIO_PIN7, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN4, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN5, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN6, GPIO_BIDIRECTIONAL_MODE),
	GPIO_PIN_CONFIG(GPIO_PORT1, GPIO_PIN7, GPIO_BIDIRECTIONAL_MODE),
};
GpioConfig led_seg7_cols = GPIO_PORT_CONFIG(GPIO_PORT2, GPIO_BIDIRECTIONAL_MODE);

uint8_t seg7_digit[SEG7_DIGIT_COUNT];
uint8_t seg7_colon;

void main() {

	serialConsoleInitialise(
		CONSOLE_UART,
		CONSOLE_SPEED, 
		CONSOLE_PIN_CONFIG
	);

	for (int i=0; i<LED_ROW_COUNT + SEG7_DIGIT_COUNT; i++){
		gpioConfigure(&led_seg7_rows[i]);
		gpioWrite(&led_seg7_rows[i], 0);
	}
	gpioConfigure(&led_seg7_cols);
	gpioWrite(&led_seg7_cols, 1);

	EA = 1;
	INT_CLKO &= ~(M_EX2 | M_EX3 | M_EX4);

	printf_tiny("INT CLKO %x!\n", (int)INT_CLKO);

	TimerStatus rc = startTimer(
		TIMER0,
		frequencyToSysclkDivisor(1000), 
		DISABLE_OUTPUT, 
		ENABLE_INTERRUPT, 
		FREE_RUNNING
	);

	printf_tiny("Run ok %d!\n", (int)rc);

	uint8_t row_index = 0;
	while (true){
		if (timer_event){
			timer_event = false;
			EA = 0;
			led_scan_row(row_index);
			EA = 1;
			printf_tiny("row: %d", (int)row_index);
			row_index++;
			if (row_index == LED_ROW_COUNT){
				row_index = 0;
			}
		}
	}
}
