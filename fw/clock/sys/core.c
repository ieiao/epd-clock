#include "core.h"
#include "timer.h"
#include "pm.h"
#include "pcf8563.h"
#include "time.h"
#include "ui.h"

void main_loop(void)
{
    static struct time time = {
        .seconds = 59,
        .minutes = 42,
        .hours = 20,
        .days = 5,
        .weekdays = 0,
        .months = 9,
        .years = 21,
    };

    pcf8563_set_time(&time);
    delay_ms(2000);
    pcf8563_get_time(&time);
    ui_main_loop(&time);
    sys_suspend();
}