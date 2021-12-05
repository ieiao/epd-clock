#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <msp430g2553.h>

enum {
    BUTTON0 = 0,
    BUTTON1,
    BUTTON2
};

enum {
    BT_BEGIN = 0,
    BT_PUSHED,
    BT_KEEP_PUSHED,
    BT_RELEASE,
};

enum {
    BT_EVENT_NONE = 0,
    BT_EVENT_SHORT,
    BT_EVENT_LONG,
};

unsigned char get_button_event(unsigned char button);
void clear_all_button_event(void);
void buttons_scan(void);

#endif
