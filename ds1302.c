
#include "ds1302.h"

#include <delay.h>

void ds1302_reset(){
    DS1302_PIN_CE = 0;
    delay10us(1);
    DS1302_PIN_CE = 1;
    delay10us(1);
}

void ds1302_write_byte(uint8_t cmd, uint8_t byte){
	uint8_t i;
    ds1302_reset();
	for (i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		DS1302_PIN_IO = cmd & 1;
		DS1302_PIN_CLK = 1;
		cmd >>= 1;
	}
    for (i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		DS1302_PIN_IO = byte & 1;
		DS1302_PIN_CLK = 1;
		byte >>= 1;
	}
	DS1302_PIN_IO = 1;
}

uint8_t ds1302_read_byte(uint8_t cmd){
	uint8_t i, byte;
    ds1302_reset();
    for (i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		DS1302_PIN_IO = cmd & 1;
		DS1302_PIN_CLK = 1;
		cmd >>= 1;
	}
	for (i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		byte >>= 1;
		byte |= (uint8_t)DS1302_PIN_IO << 7;
		DS1302_PIN_CLK = 1;
	}
	return byte;
}


