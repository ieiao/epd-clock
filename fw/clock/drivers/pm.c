#include "pm.h"
#include "gpio.h"

void sys_suspend(void)
{
    P1SEL2 = 0x00;
    P1SEL = 0x00;
    P1DIR |= BIT1 + BIT2;
    P1OUT |= BIT1 + BIT2;

    P3DIR = 0xff;
    P3OUT = 0xff;
    SYS_ENTER_SLEEP;
    gpio_resume();
}