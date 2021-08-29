#include <msp430g2553.h>

void uart_init(void)
{
    /* SMCLK */
    UCA0CTL1 |= UCSSEL_2;

    /* baud rate = 9600 */
    UCA0BR0 = 833%256;
    UCA0BR1 = 833/256;
    UCA0MCTL = UCBRS_2;
    UCA0CTL1 &= ~UCSWRST;
}

char uart_getc(void)
{
    while(!(IFG2&UCA0RXIFG));
    return UCA0RXBUF;
}

void uart_putc(char c)
{
    while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = c;
}

void uart_write(char *p)
{
    while(*p != '\0') {
        while (!(IFG2&UCA0TXIFG));
        if (*p == '\n') {
            UCA0TXBUF = *p++;
            while (!(IFG2&UCA0TXIFG));
            UCA0TXBUF = '\r';
        } else
            UCA0TXBUF = *p++;
    }
}