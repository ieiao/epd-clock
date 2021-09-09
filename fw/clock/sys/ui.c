#include "ui.h"
#include "font.h"
#include "epd-2in9.h"
#include "spi-nor.h"
#include "time.h"
#include "utf8-cov.h"
#include "uart.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void main_page_draw(void *data);
void main_page_ctl(void *data);
void setting_page_draw(void *data);
void setting_page_ctl(void *data);

const struct user_page top_pages[] = {
    {
        .draw = main_page_draw,
        .ctl = main_page_ctl,
        .next = (struct user_page *)&top_pages[1],
        .prev = (struct user_page *)&top_pages[1],
    },
    {
        .draw = setting_page_draw,
        .ctl = setting_page_ctl,
        .next = (struct user_page *)&top_pages[0],
        .prev = (struct user_page *)&top_pages[0],
    }
};

void __dot_matrix_copy(uint32_t offset, uint32_t length)
{
    uint32_t remain = length;
    char buffer[32];

    while (remain > 0) {
        if (remain >= 32) {
            spi_nor_read(offset, 32, buffer);
            epd_write_ram(buffer, 32);
            offset += 32;
            remain -= 32;
        } else {
            spi_nor_read(offset, remain, buffer);
            epd_write_ram(buffer, remain);
            offset += remain;
            remain -= remain;
        }
    }
}

void single_chinese_dot_matrix_copy(uint32_t offset)
{
    __dot_matrix_copy(offset, 32);
}

void single_ascii_dot_matrix_copy(uint32_t offset)
{
    __dot_matrix_copy(offset, 16);
}

void draw_utf8_string(const char *p)
{
    int32_t unicode;
    uint32_t offset;

    while (*p != '\0') {
        unicode = utf8_cov_unicode(p, &offset);
        p += offset;
        if (unicode < 128) {
            offset = get_ascii_offset(unicode);
            single_ascii_dot_matrix_copy(offset);
        } else {
            offset = get_chinese_offset(unicode);
            single_chinese_dot_matrix_copy(offset);
        }
    }
}

const char year_string[] = "2000年";
const char month_string[] = "01月";
const char day_string[] = "01日";
const char weekday_string[] = "星期";
const char *weekday_strings[] = {
    "日", "一", "二", "三", "四", "五", "六"
};

void assemble_data_string(char *buffer, struct time *time)
{
    strcpy(buffer, year_string);
    buffer[2] = time->years/10 + 0x30;
    buffer[3] = time->years%10 + 0x30;
    buffer += strlen(year_string);

    strcpy(buffer, month_string);
    buffer[0] = time->months/10 + 0x30;
    buffer[1] = time->months%10 + 0x30;
    buffer += strlen(month_string);

    strcpy(buffer, day_string);
    buffer[0] = time->days/10 + 0x30;
    buffer[1] = time->days%10 + 0x30;
}

void assemble_weekday_string(char *buffer, struct time *time)
{
    strcpy(buffer, weekday_string);
    buffer += strlen(weekday_string);
    strcpy(buffer, weekday_strings[time->weekdays]);
}

void draw_time(struct time *time)
{
    uint32_t offset, index;
    char buffer[32];

    epd_set_window(24, 0, 119, 295);

    index = time->hours/10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768);

    index = time->hours%10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768);

    index = 10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 480);

    index = time->minutes/10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768);

    index = time->minutes%10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768);

    epd_set_window(8, 0, 23, 111);
    assemble_data_string(buffer, time);
    draw_utf8_string(buffer);

    epd_set_window(8, 247, 23, 295);
    assemble_weekday_string(buffer, time);
    draw_utf8_string(buffer);

    epd_turnon();
}

void main_page_draw(void *data)
{
    draw_time((struct time *)data);
}

void main_page_ctl(void *data)
{

}

void setting_page_draw(void *data)
{

}

void setting_page_ctl(void *data)
{

}

void ui_main_loop(void *data)
{
    main_page_draw(data);
}