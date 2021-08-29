#ifndef _SPI_H_
#define _SPI_H_

#include <stdbool.h>

#define CS_SPI_NOR 0
#define CS_EPD_DIS 1

char __spi_rw_byte(char c);
void spi_transfer(char *w_buf, char *r_buf, char cs, int length, bool end_transfer);

#endif