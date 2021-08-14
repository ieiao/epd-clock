#include <msp430g2553.h>
#include <stdio.h>
#include <stdlib.h>
#include "serprog.h"
#include "hal/uart.h"
#include "hal/gpio.h"
#include "hal/spi.h"

#define PGMNAME "msp-serprog"
#define CMD_MAP (\
    (1L << S_CMD_NOP)        | \
    (1L << S_CMD_Q_IFACE)    | \
    (1L << S_CMD_Q_CMDMAP)   | \
    (1L << S_CMD_Q_PGMNAME)  | \
    (1L << S_CMD_Q_SERBUF)   | \
    (1L << S_CMD_Q_BUSTYPE)  | \
    (1L << S_CMD_SYNCNOP)    | \
    (1L << S_CMD_S_BUSTYPE)  | \
    (1L << S_CMD_O_SPIOP)      \
)
#define BUS_SPI (1 << 3)

void handle_command(int8_t c)
{
    u_int32_t i = 0;
    u_int32_t slen;
    u_int32_t rlen;

    switch(c) {
    case S_CMD_NOP:
        uart_putc(S_ACK);
        break;

    case S_CMD_Q_IFACE:
        uart_putc(S_ACK);
        uart_putc(0x1);
        uart_putc(0x0);
        break;

    case S_CMD_Q_CMDMAP:
        uart_putc(S_ACK);
        uart_putc(CMD_MAP & 0xff);
        uart_putc((CMD_MAP >> 8) & 0xff);
        uart_putc((CMD_MAP >> 16) & 0xff);
        uart_putc((CMD_MAP >> 24) & 0xff);
        for (i = 0; i < (32-4); i++)
            uart_putc(0x0);
        break;
    
    case S_CMD_Q_PGMNAME:
        uart_putc(S_ACK);
        while (PGMNAME[i])
        {
            uart_putc(PGMNAME[i]);
            i++;
        }
        for (; i < 16; i++)
            uart_putc(0x00);
        break;
    
    case S_CMD_Q_SERBUF:
        uart_putc(S_ACK);
        uart_putc(0xff);
        uart_putc(0xff);
        break;
    
    case S_CMD_Q_BUSTYPE:
        uart_putc(S_ACK);
        uart_putc(BUS_SPI);
        break;

    case S_CMD_SYNCNOP:
        uart_putc(S_NAK);
        uart_putc(S_ACK);
        break;

    case S_CMD_S_BUSTYPE:
        if ( (uart_getc()|BUS_SPI) == BUS_SPI)
            uart_putc(S_ACK);
        else
            uart_putc(S_NAK);
        break;

    case S_CMD_O_SPIOP:
        uart_putc(S_ACK);
        slen = uart_getc() | (uart_getc() << 8) | (((u_int32_t)uart_getc()) << 16);
        rlen = uart_getc() | (uart_getc() << 8) | (((u_int32_t)uart_getc()) << 16);

        P1OUT &= ~BIT3;
        if (slen) {
            for (i = 0; i < slen; i++)
                __spi_rw_byte(uart_getc());
        }
        if (rlen) {
            for (i = 0; i < rlen; i++)
                uart_putc(__spi_rw_byte(0xff));
        }
        P1OUT |= BIT3;
        break;

    default:
        uart_putc(S_NAK);
        break;
    }
}

void main()
{
    volatile char i;

    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    if (CALBC1_16MHZ == 0xFF)  // If calibration constant erased
    {
        while (1)
            ; // do not load, trap CPU!!
    }
    DCOCTL = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ; // Set DCO
    DCOCTL = CALDCO_16MHZ;

    gpio_init();
    uart_init();

    while (1)
        handle_command(uart_getc());
}