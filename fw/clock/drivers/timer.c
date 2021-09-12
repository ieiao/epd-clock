#include "timer.h"
#include "button.h"

volatile static uint32_t sys_counter;
volatile static uint32_t delay_count;
volatile static uint8_t buttons_count;

void timer_init(void)
{
    /* 使用TA用做延时, 精度1ms，调整CCR0可以调整精度，需要同时调整icnt匹配关系 */
    /* SMCLK 8M, TA input 8 divide */
    CCR0 = 1000;
    TACCTL0 = CCIE;
    TACTL = ID_3 + TASSEL_2 + MC_1;
}

void delay_ms(uint32_t i)
{
    delay_count = sys_counter + i;
    __bis_SR_register(CPUOFF);
    delay_count = 0;
}

void timer_suspend(void)
{
    TACTL = ID_3 + TASSEL_2 + MC_0;
    sys_counter = 0;
    buttons_count = 0;
}

void timer_resume(void)
{
    timer_init();
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
    sys_counter ++;
    if (delay_count != 0 && sys_counter >= delay_count)
        __bic_SR_register_on_exit(CPUOFF);

    buttons_count ++;
    if (buttons_count >= 20) {
        buttons_count = 0;
        buttons_scan();
    }
}