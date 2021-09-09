#include <msp430g2553.h>

volatile int ta_icnt, target_icnt;

void timer_init(void)
{
    /* 使用TA用做延时, 精度1ms，调整CCR0可以调整精度，需要同时调整icnt匹配关系 */
    /* SMCLK 8M, TA input 8 divide */
    CCR0 = 1000;
    TACCTL0 = CCIE;
    TACTL = ID_3 + TASSEL_2 + MC_0;
}

void delay_ms(int i)
{
    ta_icnt = 0;
    target_icnt = i;
    TACTL = ID_3 + TASSEL_2 + MC_1;
    __bis_SR_register(CPUOFF);
    TACTL = ID_3 + TASSEL_2 + MC_0;
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    ta_icnt ++;
    if (ta_icnt >= target_icnt)
        __bic_SR_register_on_exit(CPUOFF);
}