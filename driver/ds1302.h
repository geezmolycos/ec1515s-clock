
#ifndef _DS1302_H
#define _DS1302_H

#include "project-defs.h"

#define DS1302_PIN_CLK P3_2
#define DS1302_PIN_IO P3_3
#define DS1302_PIN_CE P3_4

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t date;
    uint8_t month;
    uint8_t day;
    uint8_t year;
    uint8_t wp;
} DS1302_regs;

#define __DS1302_GETSET(name, type, field, from, to) \
inline type ds1302_regs_get_##name(DS1302_regs *regs){return regs->field >> to & (1 << from-to) - 1;} \
inline void ds1302_regs_set_##name(DS1302_regs *regs, type name){regs->field &= ~((1 << from-to) - 1 << to); regs->field |= (name & (1 << from-to) - 1) << to;}

__DS1302_GETSET(ch, bool, second, 8, 7)
__DS1302_GETSET(bcd_second, uint8_t, second, 7, 0)
__DS1302_GETSET(bcd_minute, uint8_t, minute, 7, 0)
__DS1302_GETSET(is_12, bool, hour, 8, 7)
__DS1302_GETSET(is_pm, bool, hour, 6, 5)
__DS1302_GETSET(bcd_hour_12, uint8_t, hour, 6, 0)
__DS1302_GETSET(bcd_hour_24, uint8_t, hour, 5, 0)
__DS1302_GETSET(bcd_date, uint8_t, date, 6, 0)
__DS1302_GETSET(bcd_month, uint8_t, month, 5, 0)
__DS1302_GETSET(bcd_day, uint8_t, day, 3, 0)
__DS1302_GETSET(bcd_year, uint8_t, year, 8, 0)
__DS1302_GETSET(wp, bool, wp, 8, 7)

void ds1302_reset();
void ds1302_write_byte(uint8_t cmd, uint8_t byte);
uint8_t ds1302_read_byte(uint8_t cmd);
void ds1302_write_registers(DS1302_regs *regs);
void ds1302_read_registers(DS1302_regs *regs);

#endif // _DS1302_H
