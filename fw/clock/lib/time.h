#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <stdbool.h>

struct time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    uint8_t weekdays;
    uint8_t months;
    uint8_t years;
};

uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day);
bool is_leap_year(uint16_t year);
void dec_to_bcd(struct time *time);
void bcd_to_dec(struct time *time);

#endif
