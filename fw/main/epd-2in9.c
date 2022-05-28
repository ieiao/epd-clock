#include "hal/spi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "hal/gpio_types.h"
#include "hal/spi_types.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "driver/spi_master.h"
#include <string.h>

#define PIN_NUM_MOSI    7
#define PIN_NUM_CLK     6
#define PIN_NUM_CS      10

#define PIN_NUM_BUSY    4
#define PIN_NUM_RST     5
#define PIN_NUM_DC      8

#define EPD_WIDTH       128
#define EPD_HEIGHT      296

#define EPD_BUF_SIZE    4736

static const unsigned char lut_full_update[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char epd_init_sequence[] = {
    0x01, ((EPD_HEIGHT-1)&0xff) , (((EPD_HEIGHT-1)>>8)&0xff), 0x00,
    0x0c, 0xd7, 0xd6, 0x9d,
    0x2c, 0xa8,
    0x3a, 0x1a,
    0x3b, 0x08,
    0x3c, 0x03,
    0x11, 0x07,
};

static spi_device_handle_t dev;
static unsigned char epd_buffer[EPD_BUF_SIZE];

static void epd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    while(gpio_get_level(PIN_NUM_BUSY) == 1)
        vTaskDelay(10/portTICK_PERIOD_MS);
    gpio_set_level(PIN_NUM_DC, dc);
}

static void epd_gpio_init(void)
{
    gpio_reset_pin(PIN_NUM_BUSY);
    gpio_reset_pin(PIN_NUM_DC);
    gpio_reset_pin(PIN_NUM_RST);

    gpio_set_direction(PIN_NUM_BUSY, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);

    gpio_set_pull_mode(PIN_NUM_BUSY, GPIO_PULLDOWN_ONLY);

    gpio_set_level(PIN_NUM_RST, 1);
}

static void epd_spi_init(void)
{
    esp_err_t ret;

    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = EPD_BUF_SIZE,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 4000000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 64,
        .pre_cb = epd_spi_pre_transfer_callback,
    };

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &dev);
    ESP_ERROR_CHECK(ret);
}

static void epd_cmd(unsigned char c)
{
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &c;
    t.user = (void *)0;
    spi_device_polling_transmit(dev, &t);
}

static void epd_dat(unsigned char d)
{
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &d;
    t.user = (void *)1;
    spi_device_polling_transmit(dev, &t);
}

static void epd_dat_n(unsigned char *d, uint32_t length)
{
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    t.length = 8 * length;
    t.tx_buffer = d;
    t.user = (void *)1;
    spi_device_polling_transmit(dev, &t);
}

void epd_set_window(int xs, int ys, int xe, int ye)
{
    epd_cmd(0x44);
    epd_dat((xs >> 3) & 0xff);
    epd_dat((xe >> 3) &0xff);

    epd_cmd(0x45);
    epd_dat(ys & 0xff);
    epd_dat((ys >> 8) & 0xff);
    epd_dat(ye & 0xff);
    epd_dat((ye >> 8) & 0xff);
}

void epd_set_cursor(int xs, int ys)
{
    epd_cmd(0x4e);
    epd_dat((xs >> 3) & 0xff);

    epd_cmd(0x4f);
    epd_dat(ys & 0xff);
    epd_dat((ys >> 8) & 0xff);
}

void epd_clear(unsigned char v)
{
    memset(epd_buffer, v, EPD_BUF_SIZE);
}

static void epd_full_screen_write(unsigned char *d)
{
    epd_set_window(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
    epd_set_cursor(0, 0);
    epd_cmd(0x24);
    epd_dat_n(d, EPD_BUF_SIZE);
}

void epd_display_update()
{
    epd_full_screen_write(epd_buffer);
    epd_cmd(0x22);
    epd_dat(0xc4);
    epd_cmd(0x20);
    epd_cmd(0xff);
}

void epd_deepsleep(void)
{
    epd_cmd(0x10);
    epd_dat(0x01);

    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_INPUT);

    gpio_set_pull_mode(PIN_NUM_BUSY, GPIO_PULLDOWN_DISABLE);
}

/*
 * This function only fit for current refresh mode
 *
 * row      : range 0 ... 15
 * column   : range 0 ... 295
 * width    : number of column
 * hegiht   : number of row
 * p        : data pointer
 */
void epd_area_fill(int row, int column, int width, int hegiht, char *p)
{
    int i;
    for (i = 0; i < hegiht; i++)
        memcpy(epd_buffer + 296 * (row + i) + column, p + (i * width), width);
}

void epd2in9_init(void)
{
    int i;

    epd_gpio_init();
    epd_spi_init();

    memset(epd_buffer, 0xff, EPD_BUF_SIZE);

    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(10/portTICK_PERIOD_MS);
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(30/portTICK_PERIOD_MS);

    for (i = 0; i < sizeof(epd_init_sequence); i++) {
        if (i == 0 || i == 4 || i == 8 || i == 10 ||
            i == 12 || i == 14 || i == 16)
            epd_cmd(epd_init_sequence[i]);
        else
            epd_dat(epd_init_sequence[i]);
    }

    epd_cmd(0x32);
    epd_dat_n((unsigned char *)lut_full_update, sizeof(lut_full_update));
}
