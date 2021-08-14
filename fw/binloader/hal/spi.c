#include <msp430g2553.h>
#include <stdio.h>

/* SPI MODE 0 */
char __spi_rw_byte(char c)
{
    char i;
    char r_temp = 0;
    /* make sure spi clk pin at mode 0 idle state before do everything */
    for (i = 0; i < 8; i++) {
        /* prepare data */
        if (c & 0x80)
            P3OUT |= BIT1;
        else
            P3OUT &= ~BIT1;
        /* rising edge, sampling data */
        P3OUT |= BIT2;
        r_temp = r_temp << 1;
        if (P3IN & BIT0)
            r_temp |= BIT0;
        /* falling edge */
        P3OUT &= ~BIT2;
        c <<= 1;
    }
    return r_temp;
}

 void spi_transfer(char *w_buf, char *r_buf, char cs, int length)
{
    int n;

    switch (cs)
    {
    case 0:
        P1OUT &= ~BIT3;
        break;
    
    default:
        break;
    }

    for (n = 0; n < length; n++) {
        if ((w_buf != NULL) && (r_buf != NULL)) {
            *r_buf = __spi_rw_byte(*w_buf);
            w_buf++;
            r_buf++;
        } else if ((w_buf == NULL) && (r_buf != NULL)) {
            *r_buf = __spi_rw_byte(0xff);
            r_buf++;
        } else if ((w_buf != NULL) && (r_buf == NULL)) {
            __spi_rw_byte(*w_buf);
            w_buf++;
        }
    }

    switch (cs)
    {
    case 0:
        P1OUT |= BIT3;
        break;
    
    default:
        break;
    }
}