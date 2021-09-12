#ifndef _PM_H_
#define _PM_H_

#include <msp430g2553.h>

#define SYS_ENTER_SLEEP __bis_SR_register(LPM4_bits)
#define SYS_EXIT_SLEEP  __bic_SR_register_on_exit(LPM4_bits)

void sys_suspend(void);

#endif