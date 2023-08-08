
#include "datetime.h"

typedef enum {
	TIME_FIELD_HOUR = 0,
	TIME_FIELD_MINUTE,
	TIME_FIELD_SECOND,
	TIME_FIELD_MODE,
	TIME_FIELD_LEADING,
	TIME_FIELD_COUNT
} TimeField;

TimeField time_adjuster_target;
uint8_t last_second = 0xff;
uint8_t colon_count_slow = 0;
typedef union {
	uint8_t a[TIME_FIELD_COUNT];
	struct {
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		uint8_t mode;
		uint8_t leading;
	} t;
} TimeStore;

TimeStore time_store;
const TimeStore time_limit = {.t={.hour=0x24, .minute=0x60, .second=0x60, .mode=0x02, .leading=0x02}};

static uint8_t bcd_increase(uint8_t bcd){
	if ((bcd & 0x0f) >= 9){
		return bcd + 7;
	} else {
		return bcd + 1;
	}
}

static uint8_t bcd_decrease(uint8_t bcd){
	if ((bcd & 0x0f) == 0){
		return bcd - 7;
	} else {
		return bcd - 1;
	}
}

static void decrease_target(){
	time_store.a[time_adjuster_target] = bcd_decrease(time_store.a[time_adjuster_target]);
	if (time_store.a[time_adjuster_target] >= time_limit.a[time_adjuster_target]){
		time_store.a[time_adjuster_target] = time_limit.a[time_adjuster_target] - 1;
	}
}

static void increase_target(){
	time_store.a[time_adjuster_target] = bcd_increase(time_store.a[time_adjuster_target]);
	if (time_store.a[time_adjuster_target] >= time_limit.a[time_adjuster_target]){
		time_store.a[time_adjuster_target] = 0;
	}
}

void time_displayer_init(bool adjust){
	time_adjuster_target = TIME_FIELD_HOUR;
	uint8_t t = ds1302_read_byte(UI_DATETIME_TIME_CONFIG_DS1302_ADDR | 0x01);
	time_store.t.mode = t & 1;
	time_store.t.leading = t >> 1 & 1;
}
void time_displayer_exit(bool adjust){
	if (adjust){
		ds1302_regs_set_is_12(&regs, false);
		ds1302_regs_set_bcd_hour_24(&regs, time_store.t.hour);
		ds1302_regs_set_bcd_minute(&regs, time_store.t.minute);
		ds1302_regs_set_bcd_second(&regs, time_store.t.second);
		ds1302_write_registers(&regs);
		ds1302_write_byte(0xc0, time_store.t.mode | time_store.t.leading << 1);
	}
}
void time_displayer(bool adjust){
	if (!adjust){
		time_store.t.hour = ds1302_regs_get_bcd_hour_24(&regs);
		time_store.t.minute = ds1302_regs_get_bcd_minute(&regs);
		time_store.t.second = ds1302_regs_get_bcd_second(&regs);
	} else {
		if (s1e.series == 0 && (s1e.type == BUTTONEVENT_DOWN || s1e.type == BUTTONEVENT_REPEAT)){
			increase_target();
			colon_count_slow = count_slow;
		}
		if (s2e.series == 0 && (s2e.type == BUTTONEVENT_DOWN || s2e.type == BUTTONEVENT_REPEAT)){
			decrease_target();
			colon_count_slow = count_slow;
		}
		if (s1e.series >= 1 && s1e.type == BUTTONEVENT_DOWN){
			decrease_target();
			time_adjuster_target++;
			if (time_adjuster_target >= TIME_FIELD_COUNT) time_adjuster_target = 0;
			colon_count_slow = count_slow;
		}
		if (s2e.series >= 1 && s2e.type == BUTTONEVENT_DOWN){
			increase_target();
			time_adjuster_target--;
			if (time_adjuster_target >= TIME_FIELD_COUNT) time_adjuster_target = TIME_FIELD_COUNT - 1;
			colon_count_slow = count_slow;
		}
	}
	if (adjust && time_adjuster_target == TIME_FIELD_HOUR && (uint8_t)(count_slow - colon_count_slow) & 0x10){
		seg_set_digit(0, 0x80);
		seg_set_digit(1, 0x80);
	} else {
		uint8_t hour = time_store.t.hour;
		if (time_store.t.mode){
			if (hour >= 0x12){
				hour = hour - 0x10;
				hour = bcd_decrease(bcd_decrease(hour));
			}
		}
		if (!time_store.t.leading && (hour & 0xf0) == 0){
			seg_set_digit(0, 0x80); // empty
		} else {
			seg_set_digit(0, (hour & 0xf0) >> 4);
		}
		seg_set_digit(1, hour & 0x0f);
	}
	if (adjust && time_adjuster_target == TIME_FIELD_MINUTE && (uint8_t)(count_slow - colon_count_slow) & 0x10){
		seg_set_digit(2, 0x80);
		seg_set_digit(3, 0x80);
	} else if (!adjust || (time_adjuster_target != TIME_FIELD_MODE && time_adjuster_target != TIME_FIELD_LEADING)) {
		seg_set_digit(2, (time_store.t.minute & 0xf0) >> 4);
		seg_set_digit(3, time_store.t.minute & 0x0f);
	}
	if (adjust && time_adjuster_target == TIME_FIELD_MODE && (uint8_t)(count_slow - colon_count_slow) & 0x10){
		if (time_store.t.mode == true){
			if (time_store.t.hour > 0x12){
				seg_set_digit(2, 0x11);
			} else {
				seg_set_digit(2, 0x0a);
			}
		} else {
			seg_set_digit(2, 0x12);
		}
		seg_set_digit(3, 0x80);
	} else if (adjust && time_adjuster_target == TIME_FIELD_MODE){
		seg_set_digit(2, 0x80);
		seg_set_digit(3, 0x80);
	}
	if (adjust && time_adjuster_target == TIME_FIELD_LEADING && (uint8_t)(count_slow - colon_count_slow) & 0x10){
		if (time_store.t.leading == true){
			seg_set_digit(2, 0x00);
			seg_set_digit(3, 0x13);
		} else {
			seg_set_digit(2, 0x80);
			seg_set_digit(3, 0x13);
		}
	} else if (adjust && time_adjuster_target == TIME_FIELD_LEADING){
		seg_set_digit(2, 0x80);
		seg_set_digit(3, 0x80);
	}
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
	if (!(adjust && time_adjuster_target == TIME_FIELD_SECOND && (uint8_t)(count_slow - colon_count_slow) & 0x10)){
		uint8_t place = ((time_store.t.second & 0xf0) >> 4) * 10 + (time_store.t.second & 0x0f);
		uint8_t byte_i = place / 8, bit_i = place % 8;
		led_seg_state[byte_i] = 1 << bit_i;
	}
}

void date_displayer(bool adjust){
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
