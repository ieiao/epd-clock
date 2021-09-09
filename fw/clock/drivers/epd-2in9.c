#include <stdio.h>
#include <stdbool.h>
#include "epd-2in9.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"

#define WIDTH    128
#define HEIGHT   296

const char epd_2in9_lut_full_update[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const char epd_2in9_lut_partial_update[] = {
    0x99, 0x99, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x8F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const char epd_2in9_init_cmd_1[] = { 0x01 };
const char epd_2in9_init_cmd_2[] = { 0x0c };
const char epd_2in9_init_cmd_3[] = { 0x2c };
const char epd_2in9_init_cmd_4[] = { 0x3a };
const char epd_2in9_init_cmd_5[] = { 0x3b };
const char epd_2in9_init_cmd_6[] = { 0x3c };
const char epd_2in9_init_cmd_7[] = { 0x11 };
const char epd_2in9_init_cmd_8[] = { 0x32 };

const char epd_2in9_init_dat_1[] = { (HEIGHT-1)&0xff, ((HEIGHT-1)>>8)&0xff, 0x00 };
const char epd_2in9_init_dat_2[] = { 0xd7, 0xd6, 0x9d };
const char epd_2in9_init_dat_3[] = { 0xa8 };
const char epd_2in9_init_dat_4[] = { 0x1a };
const char epd_2in9_init_dat_5[] = { 0x08 };
const char epd_2in9_init_dat_6[] = { 0x03 };
const char epd_2in9_init_dat_7[] = { 0x03 };

const char epd_2in9_cmd_set_ramx_area[] = { 0x44 };
const char epd_2in9_cmd_set_ramy_area[] = { 0x45 };
const char epd_2in9_cmd_set_ramx[] = { 0x4e };
const char epd_2in9_cmd_set_ramy[] = { 0x4f };
const char epd_2in9_cmd_write_ram[] = { 0x24 };
const char epd_2in9_cmd_master_activation[] = { 0x20 };
const char epd_2in9_cmd_nop[] = { 0xff };

const char epd_2in9_cmd_dis_update_c2[] = { 0x22 };
const char epd_2in9_dat_dis_update_c2[] = { 0xc4 };

const char epd_2in9_cmd_deep_sleep[] = { 0x10 };
const char epd_2in9_dat_deep_sleep[] = { 0x01 };

void epd_wait_until_idle(void)
{
    while(EPD_BUSY_STATE != 0)
        delay_ms(10);
}

void epd_reset(void)
{
    EPD_RESET_SET;
    delay_ms(20);
    EPD_RESET_RESET;
    delay_ms(10);
    EPD_RESET_SET;
    delay_ms(20);
}

void epd_write_command(const char *command, int length)
{
    EPD_DC_RESET;
    spi_transfer((char *)command, NULL, CS_EPD_DIS, length, true);
}

void epd_write_data(const char *data, uint32_t length)
{
    EPD_DC_SET;
    spi_transfer((char *)data, NULL, CS_EPD_DIS, length, true);
}

void epd_init(int mode)
{
    epd_reset();

    epd_write_command(epd_2in9_init_cmd_1, sizeof(epd_2in9_init_cmd_1));
    epd_write_data(epd_2in9_init_dat_1, sizeof(epd_2in9_init_dat_1));

    epd_write_command(epd_2in9_init_cmd_2, sizeof(epd_2in9_init_cmd_2));
    epd_write_data(epd_2in9_init_dat_2, sizeof(epd_2in9_init_dat_2));

    epd_write_command(epd_2in9_init_cmd_3, sizeof(epd_2in9_init_cmd_3));
    epd_write_data(epd_2in9_init_dat_3, sizeof(epd_2in9_init_dat_3));

    epd_write_command(epd_2in9_init_cmd_4, sizeof(epd_2in9_init_cmd_4));
    epd_write_data(epd_2in9_init_dat_4, sizeof(epd_2in9_init_dat_4));

    epd_write_command(epd_2in9_init_cmd_5, sizeof(epd_2in9_init_cmd_5));
    epd_write_data(epd_2in9_init_dat_5, sizeof(epd_2in9_init_dat_5));

    epd_write_command(epd_2in9_init_cmd_6, sizeof(epd_2in9_init_cmd_6));
    epd_write_data(epd_2in9_init_dat_6, sizeof(epd_2in9_init_dat_6));

    epd_write_command(epd_2in9_init_cmd_7, sizeof(epd_2in9_init_cmd_7));
    epd_write_data(epd_2in9_init_dat_7, sizeof(epd_2in9_init_dat_7));

    epd_write_command(epd_2in9_init_cmd_8, sizeof(epd_2in9_init_cmd_8));
    if (mode == EPD_MODE_FULL_REFRESH)
        epd_write_data(epd_2in9_lut_full_update, sizeof(epd_2in9_lut_full_update));
    else
        epd_write_data(epd_2in9_lut_partial_update, sizeof(epd_2in9_lut_partial_update));
}

void epd_deep_sleep(void)
{
    epd_write_command(epd_2in9_cmd_deep_sleep, sizeof(epd_2in9_cmd_deep_sleep));
    epd_write_data(epd_2in9_dat_deep_sleep, sizeof(epd_2in9_dat_deep_sleep));
}

void epd_set_cursor(int xs, int ys)
{
    char buffer[2];
    epd_write_command(epd_2in9_cmd_set_ramx, sizeof(epd_2in9_cmd_set_ramx));
    buffer[0] = (xs >> 3) & 0xff;
    epd_write_data(buffer, 1);

    epd_write_command(epd_2in9_cmd_set_ramy, sizeof(epd_2in9_cmd_set_ramy));
    buffer[0] = ys & 0xff;
    buffer[1] = (ys >> 8) & 0xff;
    epd_write_data(buffer, 2);
}

void epd_set_window(int xs, int ys, int xe, int ye)
{
    char buffer[4];
    epd_write_command(epd_2in9_cmd_set_ramx_area, sizeof(epd_2in9_cmd_set_ramx_area));
    buffer[0] = (xs >> 3) & 0xff;
    buffer[1] = (xe >> 3) & 0xff;
    epd_write_data(buffer, 2);

    epd_write_command(epd_2in9_cmd_set_ramy_area, sizeof(epd_2in9_cmd_set_ramy_area));
    buffer[0] = ys & 0xff;
    buffer[1] = (ys >> 8) & 0xff;
    buffer[2] = ye & 0xff;
    buffer[3] = (ye >> 8) & 0xff;
    epd_write_data(buffer, 4);

    epd_set_cursor(xs, ys);
}

void epd_write_ram(const char *p, uint32_t length)
{
    epd_write_command(epd_2in9_cmd_write_ram, sizeof(epd_2in9_cmd_write_ram));
    epd_write_data(p, length);
}

void epd_turnon(void)
{
    epd_set_window(0, 0, WIDTH-1, HEIGHT-1);
    epd_write_command(epd_2in9_cmd_dis_update_c2, sizeof(epd_2in9_cmd_dis_update_c2));
    epd_write_data(epd_2in9_dat_dis_update_c2, sizeof(epd_2in9_dat_dis_update_c2));
    epd_write_command(epd_2in9_cmd_master_activation, sizeof(epd_2in9_cmd_master_activation));
    epd_write_command(epd_2in9_cmd_nop, sizeof(epd_2in9_cmd_nop));
    epd_wait_until_idle();
}

void epd_clear(void)
{
    epd_set_window(0, 0, WIDTH-1, HEIGHT-1);
    epd_write_ram(NULL, 4736);
}

void epd_draw(const char *p)
{
    volatile int i;
    unsigned int w, h;

    w = WIDTH/8;
    h = HEIGHT;

    epd_set_window(0, 0, WIDTH, HEIGHT);
    for (i = 0; i< h; i++) {
        epd_set_cursor(0, i);
        epd_write_command(epd_2in9_cmd_write_ram, sizeof(epd_2in9_cmd_write_ram));
        epd_write_data(p, w);
        p += w;
    }
}