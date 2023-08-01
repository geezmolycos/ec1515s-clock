
#include "ds1302.h"

#include <delay.h>

void ds1302_reset(){
    DS1302_PIN_CE = 0;
    delay10us(1);
    DS1302_PIN_CE = 1;
    delay10us(1);
}

static void ds1302_send_byte(uint8_t byte){
    for (uint8_t i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		DS1302_PIN_IO = byte & 1;
		DS1302_PIN_CLK = 1;
		byte >>= 1;
	}
}

static uint8_t ds1302_recv_byte(){
    uint8_t byte;
    DS1302_PIN_IO = 1;
    for (uint8_t i=0; i<8; i++){
		DS1302_PIN_CLK = 0;
		byte >>= 1;
		byte |= (uint8_t)DS1302_PIN_IO << 7;
		DS1302_PIN_CLK = 1;
	}
    return byte;
}

void ds1302_write_byte(uint8_t cmd, uint8_t byte){
    ds1302_reset();
	ds1302_send_byte(cmd);
    ds1302_send_byte(byte);
	DS1302_PIN_IO = 1;
}

uint8_t ds1302_read_byte(uint8_t cmd){
    ds1302_reset();
    ds1302_send_byte(cmd);
	return ds1302_recv_byte();
}

void ds1302_write_registers(DS1302_regs *regs){
    ds1302_reset();
    ds1302_send_byte(0xbe);
    for (uint8_t i=0; i<8; i++){
        ds1302_send_byte(((uint8_t *)regs)[i]);
    }
}

void ds1302_read_registers(DS1302_regs *regs){
    ds1302_reset();
    ds1302_send_byte(0xbf);
    for (uint8_t i=0; i<8; i++){
        ((uint8_t *)regs)[i] = ds1302_recv_byte();
    }
}
