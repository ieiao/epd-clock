#ifndef _SPI_H_
#define _SPI_H_

#include <stdbool.h>
#include <stdint.h>

enum {
    CS_SPI_NOR = 0,
    CS_EPD_DIS
};

char __spi_rw_byte(char c);
void spi_transfer(char *w_buf, char *r_buf, char cs, uint32_t length, bool end_transfer);

#endif