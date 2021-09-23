#ifndef _CORE_H_
#define _CORE_H_

#include "time.h"

enum {
    WAKEUP_NONE = 0,
    WAKEUP_BY_RTC,
    WAKEUP_BY_BUTTON
};

struct sys_info {
    struct time time;
    unsigned char wakeup_type;
};

void set_wakeup_type(unsigned char type);
void main_loop(void);

#endif
