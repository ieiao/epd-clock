#include <msp430g2553.h>

void gpio_init(void)
{
    /* P1.3 = FLASHCS, P1.4 = AIN_C 初始化状态 */
    P1DIR = BIT3 + BIT4;
    P1OUT = BIT3 + BIT4;

    /* P1.1 = RXD
     * P1.2 = TXD clock串口需要关掉
     * P1.6 = SCL
     * P1.7 = SDA
     */
    P1SEL = BIT1 + BIT2 + BIT6 + BIT7;
    P1SEL2 = BIT1 + BIT2 + BIT6 + BIT7;

    P1IE |= BIT5;
    P1IES |= BIT5;
    P1IFG &= ~BIT5;

    /*
     * P3.0 = MISO              I
     * P3.1 = MOSI              O
     * P3.2 = CLK               O
     * P3.3 = EPD_PWR_EN#       O
     * P3.4 = EPD_BUSY          I
     * P3.5 = EPD_RST#          O
     * P3.6 = EPD_DC#           O
     * P3.7 = EPC_CS#           O
     */
    P3DIR = BIT1 + BIT2 + BIT3 + BIT5 + BIT6 + BIT7;

    /* 设置EPD CS和PWR MOSFET状态, 同时让CLK处于 SPI mode 0 idle状态 */
    P3OUT = BIT3 + BIT7;
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
    __bic_SR_register_on_exit(LPM3_bits);
}