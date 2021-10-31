#ifndef _CORE_H_
#define _CORE_H_

#include "time.h"
#include <stdint.h>

enum {
    WAKEUP_NONE = 0,
    WAKEUP_BY_RTC,
    WAKEUP_BY_BUTTON
};

struct sys_info {
    struct time time;
    uint8_t wakeup_type;
    int8_t night_begin;
    int8_t night_end;
    int16_t bat_val;
};

void set_wakeup_type(unsigned char type);
bool in_night_freeze(void);
void main_loop(void);

#endif
