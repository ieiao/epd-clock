#include "core.h"
#include "timer.h"
#include "pm.h"
#include "pcf8563.h"
#include "time.h"
#include "ui.h"
#include "uart.h"
#include "epd-2in9.h"
#include "gpio.h"
#include "spi-nor.h"
#include <stdio.h>
#include "button.h"

void main_loop(void)
{
    static struct time time = {
        .seconds = 45,
        .minutes = 47,
        .hours = 21,
        .days = 12,
        .weekdays = 0,
        .months = 9,
        .years = 21,
    };

    pcf8563_clr_and_en_alarm_int();
    pcf8563_set_time(&time);

    while(1) {
        epd_init(EPD_MODE_FULL_REFRESH);
        epd_clear();
        pcf8563_set_and_en_minute_alarm((time.minutes == 59) ? 0 : (time.minutes+1));
        ui_main_loop(&time);
        epd_deep_sleep();
        spi_nor_power_down();
        timer_suspend();
        sys_suspend();
        timer_resume();
        spi_nor_release_power_down();
        pcf8563_clr_and_en_alarm_int();
        pcf8563_get_time(&time);
    }
}