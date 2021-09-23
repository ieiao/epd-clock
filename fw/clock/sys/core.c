#include "timer.h"
#include "pm.h"
#include "pcf8563.h"
#include "ui.h"
#include "uart.h"
#include "epd-2in9.h"
#include "gpio.h"
#include "spi-nor.h"
#include <stdio.h>
#include "button.h"
#include "core.h"

static struct sys_info sys_info;

void set_wakeup_type(unsigned char type)
{
    sys_info.wakeup_type = type;
}

void main_loop(void)
{
    sys_info.time.years = 21;
    sys_info.time.months = 1;
    sys_info.time.days = 1;
    sys_info.time.weekdays = calc_weekday(sys_info.time.years + 2000,
                                          sys_info.time.months,
                                          sys_info.time.days);

    pcf8563_clr_and_en_alarm_int();
    pcf8563_set_time(&sys_info.time);

    while(1) {

        ui_main_loop(&sys_info);

        pcf8563_set_and_en_minute_alarm((sys_info.time.minutes == 59) ? 0 : (sys_info.time.minutes+1));
        epd_deep_sleep();
        spi_nor_power_down();
        timer_suspend();
        set_wakeup_type(WAKEUP_NONE);
        clear_all_button_event();
        ENABLE_BUTTON_WAKEUP;
        sys_suspend();

        timer_resume();
        spi_nor_release_power_down();
        pcf8563_clr_and_en_alarm_int();
        pcf8563_get_time(&sys_info.time);
    }
}
