#include "../drivers/i2c.h"
#include "time.h"
#include <string.h>
#include "pcf8563.h"
#include <stdint.h>

void pcf8563_set_time(struct time *time)
{
    struct time t_tmp;

    memcpy(&t_tmp, time, sizeof(struct time));
    dec_to_bcd(&t_tmp);
    i2c_master_write(PCF8563_ADDR, 0x02, (char *)&t_tmp, sizeof(struct time));
}

void pcf8563_get_time(struct time *time)
{
    char buffer[7];

    i2c_master_read(PCF8563_ADDR, 0x02, buffer, sizeof(buffer));
    time->seconds = buffer[0]&0x7f;
    time->minutes = buffer[1]&0x7f;
    time->hours = buffer[2]&0x3f;
    time->days = buffer[3]&0x3f;
    time->weekdays = buffer[4]&0x07;
    time->months = buffer[5]&0x1f;
    time->years = buffer[6];
    bcd_to_dec(time);
}

void pcf8563_clr_and_en_alarm_int(void)
{
    const char pcf8563_clr_and_en_alarm_int_value = 0x02;
    i2c_master_write(PCF8563_ADDR, 0x01, (char *)&pcf8563_clr_and_en_alarm_int_value, 1);
}

void pcf8563_set_and_en_minute_alarm(uint8_t minute)
{
    minute = (minute%10) | ((minute/10)<<4);
    i2c_master_write(PCF8563_ADDR, 0x09, (char *)&minute, 1);
}