#include <msp430g2553.h>
#include <stdio.h>
#include <stdbool.h>
#include "spi.h"
#include "gpio.h"

/* SPI MODE 0 */
char __spi_rw_byte(char c)
{
    char i;
    char r_temp = 0;
    /* make sure spi clk pin at mode 0 idle state before do everything */
    for (i = 0; i < 8; i++) {
        /* prepare data */
        if (c & 0x80)
            P3OUT |= BIT4;
        else
            P3OUT &= ~BIT4;
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

void spi_transfer(char *w_buf, char *r_buf, char cs, uint32_t length, bool end_transfer)
{
    uint32_t n;

    switch (cs)
    {
    case CS_SPI_NOR:
        SPI_NOR_CS_RESET;
        break;

    case CS_EPD_DIS:
        EPD_CS_RESET;
        break;
    
    default:
        break;
    }

    if (w_buf != NULL) {
        if (r_buf != NULL) {
            for (n = 0; n < length; n++) {
                *r_buf = __spi_rw_byte(*w_buf);
                w_buf++;
                r_buf++;
            }
        } else {
            for (n = 0; n < length; n++) {
                __spi_rw_byte(*w_buf);
                w_buf++;
            }
        }
    } else {
        if (r_buf != NULL) {
            for (n = 0; n < length; n++) {
                *r_buf = __spi_rw_byte(0xff);
                r_buf++;
            }
        } else {
            for (n = 0; n < length; n++) {
                __spi_rw_byte(0xff);
            }
        }
    }

    if (!end_transfer)
        return;

    switch (cs)
    {
    case CS_SPI_NOR:
        SPI_NOR_CS_SET;
        break;

    case CS_EPD_DIS:
        EPD_CS_SET;
        break;
    
    default:
        break;
    }
}