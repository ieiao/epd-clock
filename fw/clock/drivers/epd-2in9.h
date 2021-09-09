#ifndef _EPD_2IN9_H_
#define _EPD_2IN9_H_

#include <stdint.h>

enum {
    EPD_MODE_FULL_REFRESH = 0,
    EPD_MODE_PART_REFRESH
};

void epd_write_command(const char *command, int length);
void epd_write_data(const char *data, uint32_t length);
void epd_init(int mode);
void epd_deep_sleep(void);
void epd_set_window(int xs, int ys, int xe, int ye);
void epd_set_cursor(int xs, int ys);
void epd_write_ram(const char *p, uint32_t length);
void epd_turnon(void);
void epd_clear(void);
void epd_draw(const char *p);

#endif