#ifndef _SPI_H_
#define _SPI_H_

char __spi_rw_byte(char c);
void spi_transfer(char *w_buf, char *r_buf, char cs, int length);

#endif