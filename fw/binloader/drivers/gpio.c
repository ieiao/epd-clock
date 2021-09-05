#include <msp430g2553.h>

void gpio_init(void)
{
    /* P1.1 = RXD
     * P1.2 = TXD clock串口需要关掉
     * P1.6 = SCL
     * P1.7 = SDA
     */
    P1SEL = BIT1 + BIT2 + BIT6 + BIT7;
    P1SEL2 = BIT1 + BIT2 + BIT6 + BIT7;

    /* P1.3 = FLASHCS, P1.4 = AIN_C 初始化状态 */
    /* 第一版电路图I2C遗漏上拉电阻，使用GPIO上拉来暂时替换 P1.6, P1.7 */
    /* 唤醒引脚需要使能内部上拉电阻 P1.5 */
    P1DIR = BIT3 + BIT4;
    P1OUT = BIT3 + BIT4 + BIT5 + (BIT6 + BIT7);
    P1REN = BIT5 + BIT6 + BIT7;

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

    /* 设置EPD CS和PWR MOSFET状态, 同时让CLK处于 SPI mode 0 idle状态 */
    P3OUT = BIT3 + BIT7;
}