#ifndef _SPI_NOR_H_
#define _SPI_NOR_H_

#include <stdint.h>

uint32_t spi_nor_read(uint32_t offset, uint32_t length, char *buffer);
void spi_nor_power_down(void);
void spi_nor_release_power_down(void);

#endif
