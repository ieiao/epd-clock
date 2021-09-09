#include <stdio.h>
#include "spi-nor.h"
#include "spi.h"

uint32_t spi_nor_read(uint32_t offset, uint32_t length, char *buffer)
{
    char w_buf[4];

    w_buf[0] = 0x03;
    w_buf[1] = (char)((offset>>16)&0xff);
    w_buf[2] = (char)((offset>>8)&0xff);
    w_buf[3] = (char)(offset&0xff);

    spi_transfer(w_buf, NULL, CS_SPI_NOR, sizeof(w_buf), false);
    spi_transfer(NULL, buffer, CS_SPI_NOR, length, true);

    return length;
}

void spi_nor_power_down(void)
{
    const char w_buf = 0xb9;
    spi_transfer((char *)&w_buf, NULL, CS_SPI_NOR, 1, true);
}

void spi_nor_release_power_down(void)
{
    const char w_buf = 0xab;
    spi_transfer((char *)&w_buf, NULL, CS_SPI_NOR, 1, true);
}