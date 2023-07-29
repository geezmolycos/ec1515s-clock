
#include "project-defs.h"
#include <delay.h>
#include <uart-hal.h>
#include <serial-console.h>
#include <stdio.h>

bool led_state(uint8_t index){
	return index & 1;
}

void main() {

	serialConsoleInitialise(
		CONSOLE_UART,
		CONSOLE_SPEED, 
		CONSOLE_PIN_CONFIG
	);

	EA = 1;

	puts("Run ok!\n");

	P1 = 0x10;
	P2 = 0x7f;
	while (1);
}
