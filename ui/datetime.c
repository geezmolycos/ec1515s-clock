
#include "datetime.h"

typedef enum {
	TIME_FIELD_HOUR = 0,
	TIME_FIELD_MINUTE,
	TIME_FIELD_SECOND,
	TIME_FIELD_COUNT
} TimeField;

TimeField time_adjuster_target;
uint8_t last_second = 0xff;
uint8_t colon_count_slow = 0;
union {
	uint8_t a[TIME_FIELD_COUNT];
	struct {
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
	} t;
} time_store;

uint8_t bcd_increase(uint8_t bcd){
	if ((bcd & 0x0f) >= 9){
		return bcd + 7;
	} else {
		return bcd + 1;
	}
}

uint8_t bcd_decrease(uint8_t bcd){
	if ((bcd & 0x0f) == 0){
		return bcd - 7;
	} else {
		return bcd - 1;
	}
}

void time_displayer_init(bool adjust){
	time_adjuster_target = TIME_FIELD_HOUR;
}
void time_displayer_exit(bool adjust){
	if (adjust){
		ds1302_regs_set_is_12(&regs, false);
		ds1302_regs_set_bcd_hour_24(&regs, time_store.t.hour);
		ds1302_regs_set_bcd_minute(&regs, time_store.t.minute);
		ds1302_regs_set_bcd_second(&regs, time_store.t.second);
		ds1302_write_registers(&regs);
	}
}
void time_displayer(bool adjust){
	if (!adjust){
		time_store.t.hour = ds1302_regs_get_bcd_hour_24(&regs);
		time_store.t.minute = ds1302_regs_get_bcd_minute(&regs);
		time_store.t.second = ds1302_regs_get_bcd_second(&regs);
	} else {
		if (s1e.series == 0 && (s1e.type == BUTTONEVENT_DOWN || s1e.type == BUTTONEVENT_REPEAT)){
			time_store.a[time_adjuster_target] = bcd_increase(time_store.a[time_adjuster_target]);
		}
		if (s2e.series == 0 && (s2e.type == BUTTONEVENT_DOWN || s2e.type == BUTTONEVENT_REPEAT)){
			time_store.a[time_adjuster_target] = bcd_decrease(time_store.a[time_adjuster_target]);
		}
	}
	seg_set_digit(0, (time_store.t.hour & 0xf0) >> 4);
	seg_set_digit(1, time_store.t.hour & 0x0f);
	seg_set_digit(2, (time_store.t.minute & 0xf0) >> 4);
	seg_set_digit(3, time_store.t.minute & 0x0f);
	if (!adjust){
		if (time_store.t.second != last_second){
			colon_count_slow = count_slow;
		}
		last_second = time_store.t.second;
		bool colon = true;
		if ((uint8_t)(count_slow - colon_count_slow) > 25){
			colon = false;
		}
		seg_set_colon(colon ? 0x03 : 0x00);
	} else {
		seg_set_colon(0x00);
	}
	for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x00;
	}
	uint8_t place = ((time_store.t.second & 0xf0) >> 4) * 10 + (time_store.t.second & 0x0f);
	uint8_t byte_i = place / 8, bit_i = place % 8;
	// putchar(0x88);
	// putchar(count_slow);
	// putchar(colon_count_slow);
	led_seg_state[byte_i] = 1 << bit_i;
}

void date_displayer(){
	uint8_t month = ds1302_regs_get_bcd_month(&regs);
	uint8_t date = ds1302_regs_get_bcd_date(&regs);
	for (uint8_t i=0; i < LED_ROW_COUNT; i++){
		led_seg_state[i] = 0x00;
	}
	seg_set_digit(0, (month & 0xf0) >> 4);
	seg_set_digit(1, month & 0x0f);
	seg_set_digit(2, (date & 0xf0) >> 4);
	seg_set_digit(3, date & 0x0f);
}
