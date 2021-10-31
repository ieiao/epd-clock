#ifndef _PCF8563_H_
#define _PCF8563_H_

#include <stdint.h>
#include "time.h"

#define PCF8563_ADDR 0x51

void pcf8563_set_time(struct time *time);
void pcf8563_get_time(struct time *time);
void pcf8563_clr_and_en_alarm_int(void);
void pcf8563_set_and_en_minute_alarm(int8_t minute);
void pcf8563_dis_minute_alarm(void);
void pcf8563_set_and_en_hour_alarm(int8_t hour);
void pcf8563_dis_hour_alarm(void);

#endif
