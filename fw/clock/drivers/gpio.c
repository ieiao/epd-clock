#include "gpio.h"

void gpio_init(void)
{
    /* 
     * P1.0 ADC 复用配置不关心
     * P1.6 = SCL
     * P1.7 = SDA
     * 其余IO无复用
     */
    P1SEL = BIT6 + BIT7;
    P1SEL2 = BIT6 + BIT7;

    /* 
     * P1.3 = FLASHCS           O
     * P1.4 = AIN_C             O
     * P1.5 = RTC_WAKE          I
     * 初始化状态
     */
    P1DIR = BIT3 + BIT4;
    P1OUT = BIT3 + BIT4;

    /* 使能RTC唤醒引脚中断，清除中断状态 */
    P1IES |= BIT5;
    P1IFG &= ~BIT5;
    P1IE |= BIT5;

    /*
     * P3.0 = MISO              I
     * P3.1 = EPD_BUSY          I
     * P3.2 = CLK               O
     * P3.3 = EPD_PWR_EN#       O
     * P3.4 = MOSI              O
     * P3.5 = EPD_RST#          O
     * P3.6 = EPD_DC#           O
     * P3.7 = EPC_CS#           O
     */
    P3DIR = BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

    /*
     * EPC_CS      = 1
     * PED_PWR_CTL = 1
     * SPI_CLK     = 0 (SPI MODE 0 IDLE status)
     */
    P3OUT = BIT3 + BIT5 + BIT7;

    /* 
     * P2 ALL input
     */
    /* P2DIR = 0x00;
    P2IE = 0x00; */
}

void gpio_suspend(void)
{
    P1SEL = 0x00;
    P1SEL2 = 0x00;
    P1DIR = 0xff;
    P1OUT = 0xff;

    P2DIR = ~(BIT0 + BIT1 + BIT2);
    P2OUT = 0xff;
    P2IFG &= BIT0 + BIT1 + BIT2;
    P2IES |= BIT0 + BIT1 + BIT2;
    P2IE |= BIT0 + BIT1 + BIT2;

    P3DIR = 0xff;
    P3OUT = 0xff;
}

void gpio_resume(void)
{
    gpio_init();
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    P1IFG &= ~BIT5;
    __bic_SR_register_on_exit(LPM4_bits);
}

// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{

}