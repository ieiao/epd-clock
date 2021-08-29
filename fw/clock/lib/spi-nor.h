#ifndef _SPI_NOR_H_
#define _SPI_NOR_H_

#include <stdint.h>

uint16_t spi_nor_read(uint32_t offset, uint16_t length, uint8_t *buffer);
void spi_nor_power_down(void);
void spi_nor_release_power_down(void);

#endif
