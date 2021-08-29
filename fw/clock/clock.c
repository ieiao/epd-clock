#include <msp430g2553.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "drivers/platform.h"
#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "drivers/spi.h"
#include "drivers/i2c.h"
#include "lib/time.h"
#include "lib/pcf8563.h"
#include "lib/spi-nor.h"

void main()
{
    static struct time time = {
        .seconds = 0,
        .minutes = 21,
        .hours = 17,
        .days = 29,
        .weekdays = 0,
        .months = 8,
        .years = 21,
    };
    char buffer[32] = {0};

    platform_init();
    gpio_init();
    i2c_init();
    uart_init();
    int_enable();

    spi_nor_power_down();
    pcf8563_set_time(&time);
    pcf8563_clr_and_en_alarm_int();

    while(1) {
        pcf8563_set_and_en_minute_alarm((time.minutes == 59) ? 0 : (time.minutes+1));
        __bis_SR_register(LPM3_bits);
        pcf8563_clr_and_en_alarm_int();
        pcf8563_get_time(&time);
        sprintf(buffer, "20%02d/%d/%d %02d:%02d\n", time.years, time.months, time.days, time.hours, time.minutes);
        uart_write(buffer);
    }
}