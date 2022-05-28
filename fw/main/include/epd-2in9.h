#ifndef __EPD_2IN9_H__
#define __EPD_2IN9_H__

void epd_clear(unsigned char v);
void epd_area_fill(int row, int column, int width, int hegiht, char *p);
void epd_display_update(void);
void epd_deepsleep(void);
void epd2in9_init(void);

#endif
