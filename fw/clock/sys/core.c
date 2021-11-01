#include "adc.h"
#include "timer.h"
#include "pm.h"
#include "pcf8563.h"
#include "ui.h"
#include "uart.h"
#include "epd-2in9.h"
#include "gpio.h"
#include "spi-nor.h"
#include <stdio.h>
#include <stdbool.h>
#include "button.h"
#include "core.h"

static struct sys_info sys_info;

void set_wakeup_type(unsigned char type)
{
    sys_info.wakeup_type = type;
}

bool use_night_freeze(void)
{
    return (sys_info.night_begin != sys_info.night_end);
}

bool in_night_freeze(void)
{
    if (use_night_freeze()) {
        if (sys_info.night_begin < sys_info.night_end)
            return (sys_info.time.hours >= sys_info.night_begin) &&
                    (sys_info.time.hours < sys_info.night_end);
        else
            return (sys_info.time.hours >= sys_info.night_begin) ||
                    (sys_info.time.hours < sys_info.night_end);
    } else {
        return false;
    }
}

void __calc_and_set_minute_alarm(void)
{
    /* TODO 这里需要考虑一下在秒接近下一分钟时延后一分钟唤醒，防止出现唤醒失败的问题 */
    pcf8563_set_and_en_minute_alarm((sys_info.time.minutes == 59) ? 0 : (sys_info.time.minutes+1));
}

void calc_and_set_rtc_alarm(void)
{
    if (in_night_freeze()) {
        if (sys_info.wakeup_type == WAKEUP_BY_BUTTON) {
            pcf8563_dis_hour_alarm();
            __calc_and_set_minute_alarm();
        } else {
            pcf8563_dis_minute_alarm();
            pcf8563_set_and_en_hour_alarm(sys_info.night_end);
        }
    } else {
        pcf8563_dis_hour_alarm();
        __calc_and_set_minute_alarm();
    }
}

void __get_bat_val(void)
{
    sys_info.bat_val = get_adc_value() * (2.5/1024.0) * 100;
}

void main_loop(void)
{
    sys_info.time.years = 21;
    sys_info.time.months = 1;
    sys_info.time.days = 1;
    sys_info.time.hours = 12;
    sys_info.time.weekdays = calc_weekday(sys_info.time.years + 2000,
                                          sys_info.time.months,
                                          sys_info.time.days);
    __get_bat_val();

    pcf8563_clr_and_en_alarm_int();
    pcf8563_set_time(&sys_info.time);

    while(1) {

        if (sys_info.time.hours == sys_info.night_end && sys_info.time.minutes == 10)
            __get_bat_val();

        ui_main_loop(&sys_info);

        calc_and_set_rtc_alarm();
        epd_deep_sleep();
        spi_nor_power_down();
        timer_suspend();
        set_wakeup_type(WAKEUP_NONE);
        clear_all_button_event();
        /* 使能RTC唤醒引脚中断，清除中断状态 */
        P1IES |= BIT5;
        P1IFG &= ~BIT5;
        P1IE |= BIT5;
        /* 使能按键唤醒引脚中断，清除中断状态 */
        P2IFG &= ~(BIT0 + BIT1 + BIT2);
        P2IES |= BIT0 + BIT1 + BIT2;
        ENABLE_BUTTON_WAKEUP;
        sys_suspend();

        timer_resume();
        spi_nor_release_power_down();
        pcf8563_clr_and_en_alarm_int();
        pcf8563_get_time(&sys_info.time);
    }
}
