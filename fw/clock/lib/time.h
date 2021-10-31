#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <stdbool.h>

struct time {
    int8_t seconds;
    int8_t minutes;
    int8_t hours;
    int8_t days;
    int8_t weekdays;
    int8_t months;
    int8_t years;
};

int8_t calc_weekday(int16_t year, int8_t month, int8_t day);
bool is_leap_year(int16_t year);
void dec_to_bcd(struct time *time);
void bcd_to_dec(struct time *time);

#endif
