#ifndef _TIMER_H_
#define _TIMER_H_

#include <msp430g2553.h>
#include <stdint.h>

void timer_init(void);
void delay_ms(uint32_t i);
void timer_suspend(void);
void timer_resume(void);

#endif