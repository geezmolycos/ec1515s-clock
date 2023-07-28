
#ifndef _PROJECT_DEFS_H
#define _PROJECT_DEFS_H

#ifdef __SDCC
	#include <STC/90C5xRD+/PDIP40.h>
#else
	#include <uni-STC/uni-STC.h>
#endif // __SDCC

#define CONSOLE_UART   UART1
#define CONSOLE_SPEED  57600UL
#define CONSOLE_PIN_CONFIG 0

// Reduce RAM footprint so we can use the medium memory model
// regardless of the MCU.
#define HAL_UARTS 1

#endif // _PROJECT_DEFS_H
