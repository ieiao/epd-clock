#include "time.h"
#include <stdbool.h>

uint8_t calc_weekday(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t c, y, weekday;

    if (month == 1 || month == 2)
        year--, month += 12;
    c = year/100;
    y = year - c * 100;
    weekday = y + y / 4 + c / 4 - 2 * c + 26 * (month + 1) / 10 + day - 1;
    while (weekday < 0)
        weekday += 7;
    weekday %= 7;
    return weekday;
}

bool is_leap_year(uint16_t year)
{
    if (year%400 == 0)
        return true;

    if ((year%4 == 0) && (year%100 != 0))
        return true;

    return false;
}

void dec_to_bcd(struct time *time)
{
    time->seconds = (time->seconds%10) | ((time->seconds/10)<<4);
    time->minutes = (time->minutes%10) | ((time->minutes/10)<<4);
    time->hours = (time->hours%10) | ((time->hours/10)<<4);
    time->weekdays = (time->weekdays%10) | ((time->weekdays/10)<<4);
    time->days = (time->days%10) | ((time->days/10)<<4);
    time->months = (time->months%10) | ((time->months/10)<<4);
    time->years = (time->years%10) | ((time->years/10)<<4);
}

void bcd_to_dec(struct time *time)
{
    time->seconds = (time->seconds&0x0f) + ((time->seconds>>4)*10);
    time->minutes = (time->minutes&0x0f) + ((time->minutes>>4)*10);
    time->hours = (time->hours&0x0f) + ((time->hours>>4)*10);
    time->weekdays = (time->weekdays&0x0f) + ((time->weekdays>>4)*10);
    time->days = (time->days&0x0f) + ((time->days>>4)*10);
    time->months = (time->months&0x0f) + ((time->months>>4)*10);
    time->years = (time->years&0x0f) + ((time->years>>4)*10);
}