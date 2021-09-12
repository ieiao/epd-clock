#include <msp430g2553.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "drivers/platform.h"
#include "drivers/pm.h"
#include "drivers/uart.h"
#include "drivers/gpio.h"
#include "drivers/spi.h"
#include "drivers/i2c.h"
#include "drivers/epd-2in9.h"
#include "drivers/timer.h"
#include "lib/time.h"
#include "lib/pcf8563.h"
#include "lib/spi-nor.h"
#include "sys/core.h"

int main()
{
    platform_init();
    gpio_init();
    i2c_init();
    int_enable();
    timer_init();
    
    main_loop();

    while(1);
    return 0;
}