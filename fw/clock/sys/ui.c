#include "ui.h"
#include "button.h"
#include "font.h"
#include "epd-2in9.h"
#include "pcf8563.h"
#include "spi-nor.h"
#include "time.h"
#include "utf8-cov.h"
#include "uart.h"
#include "core.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void main_page_draw(void *data);
int main_page_ctl(void *data);
void setting_page_init(void *data);
void setting_page_draw(void *data);
int setting_page_ctl(void *data);

const struct user_page top_pages[] = {
    {
        .draw = main_page_draw,
        .ctl = main_page_ctl,
        .next = (struct user_page *)&top_pages[1],
        .prev = (struct user_page *)&top_pages[1],
    },
    {
        .init = setting_page_init,
        .draw = setting_page_draw,
        .ctl = setting_page_ctl,
        .next = (struct user_page *)&top_pages[0],
        .prev = (struct user_page *)&top_pages[0],
    }
};

void __buffer_reverse(char *buffer, uint32_t length)
{
    uint32_t i;
    for (i = 0; i < length; i++) {
        buffer[i] = ~buffer[i];
    }
}

void __dot_matrix_copy(uint32_t offset, uint32_t length, bool reverse_color)
{
    uint32_t remain = length;
    char buffer[32];

    while (remain > 0) {
        if (remain >= 32) {
            spi_nor_read(offset, 32, buffer);
            if (reverse_color)
                __buffer_reverse(buffer, 32);
            epd_write_ram(buffer, 32);
            offset += 32;
            remain -= 32;
        } else {
            spi_nor_read(offset, remain, buffer);
            if (reverse_color)
                __buffer_reverse(buffer, remain);
            epd_write_ram(buffer, remain);
            offset += remain;
            remain -= remain;
        }
    }
}

void single_chinese_dot_matrix_copy(uint32_t offset, bool reverse_color)
{
    __dot_matrix_copy(offset, 32, reverse_color);
}

void single_ascii_dot_matrix_copy(uint32_t offset, bool reverse_color)
{
    __dot_matrix_copy(offset, 16, reverse_color);
}

void draw_utf8_string(const char *p, bool reverse_color)
{
    int32_t unicode;
    uint32_t offset;

    while (*p != '\0') {
        unicode = utf8_cov_unicode(p, &offset);
        p += offset;
        if (unicode < 128) {
            offset = get_ascii_offset(unicode);
            single_ascii_dot_matrix_copy(offset, reverse_color);
        } else {
            offset = get_chinese_offset(unicode);
            single_chinese_dot_matrix_copy(offset, reverse_color);
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
const char hour_string[] = "01时";
const char minute_string[] = "01分";

char *__assemble_string_date(char *buffer, struct time *time)
{
    strcpy(buffer, year_string);
    buffer[2] = time->years/10 + 0x30;
    buffer[3] = time->years%10 + 0x30;
    buffer += sizeof(year_string) - 1;

    strcpy(buffer, month_string);
    buffer[0] = time->months/10 + 0x30;
    buffer[1] = time->months%10 + 0x30;
    buffer += sizeof(month_string) - 1;

    strcpy(buffer, day_string);
    buffer[0] = time->days/10 + 0x30;
    buffer[1] = time->days%10 + 0x30;
    buffer += sizeof(day_string) - 1;

    return buffer;
}

char *__assemble_string_time(char *buffer, struct time *time)
{
    strcpy(buffer, hour_string);
    buffer[0] = time->hours/10 + 0x30;
    buffer[1] = time->hours%10 + 0x30;
    buffer += sizeof(hour_string) - 1;

    strcpy(buffer, minute_string);
    buffer[0] = time->minutes/10 + 0x30;
    buffer[1] = time->minutes%10 + 0x30;
    buffer += sizeof(minute_string) - 1;

    return buffer;
}

char *__assemble_string_weekday(char *buffer, struct time *time)
{
    strcpy(buffer, weekday_string);
    buffer += sizeof(weekday_string) - 1;
    strcpy(buffer, weekday_strings[time->weekdays]);
    return buffer;
}

void __assemble_date_string(char *buffer, struct time *time)
{
    buffer = __assemble_string_date(buffer, time);
    __assemble_string_weekday(buffer, time);
}

void main_page_draw(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;
    struct time *time = &sys_info->time;
    uint32_t offset, index;
    char buffer[64];

    epd_init(EPD_MODE_FULL_REFRESH);
    epd_clear();
    epd_set_window(24, 0, 119, 295);

    index = time->hours/10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768, false);

    index = time->hours%10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768, false);

    index = 10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 480, false);

    index = time->minutes/10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768, false);

    index = time->minutes%10;
    offset = MAIN_CLOCK_FONT_OFFSET + 768*index;
    __dot_matrix_copy(offset, 768, false);

    epd_set_window(8, 68, 23, 227);
    __assemble_date_string(buffer, time);
    draw_utf8_string(buffer, false);

    epd_turnon();

}

int main_page_ctl(void *data)
{
    if (get_button_event(BUTTON0) != BT_EVENT_NONE ||
        get_button_event(BUTTON1) != BT_EVENT_NONE ||
        get_button_event(BUTTON2) != BT_EVENT_NONE)
        return PAGE_NEXT;
    return PAGE_BREAK;
}

static uint8_t setting_page_indicator;
enum {
    INDICATOR_YEAR_HI = 0,
    INDICATOR_YEAR_LO,
    INDICATOR_MONTH_HI,
    INDICATOR_MONTH_LO,
    INDICATOR_DAY_HI,
    INDICATOR_DAY_LO,
    INDICATOR_HOUR_HI,
    INDICATOR_HOUR_LO,
    INDICATOR_MINUTE_HI,
    INDICATOR_MINUTE_LO,
};

const unsigned char indicator_offset[] = {
    2, 3, 6, 7, 10, 11, 14, 15, 18, 19
};

void __setting_page_assemble_indicator(char *buffer, uint8_t indicator)
{
    memset(buffer, ' ', indicator_offset[indicator]);
    buffer += indicator_offset[indicator];
    *buffer++ = '^';
    *buffer = '\0';
}

void __setting_page_draw(char *buffer, struct time *time)
{
    epd_clear();
    epd_set_window(104, 132, 119, 163);
    draw_utf8_string("设置", false);

    epd_set_window(80, 60, 95, 235);
    __assemble_string_date(buffer, time);
    draw_utf8_string(buffer, false);
    __assemble_string_time(buffer, time);
    draw_utf8_string(buffer, false);

    if (setting_page_indicator <= INDICATOR_MINUTE_LO) {
        epd_set_window(64, 60, 79, 235);
        __setting_page_assemble_indicator(buffer, setting_page_indicator);
        draw_utf8_string(buffer, false);
    }
}

void __setting_page_adjust_days(struct time *time)
{
    switch (time->months) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (time->days > 31) {
            if (setting_page_indicator == INDICATOR_DAY_HI)
                time->days -= 40;
            else if(setting_page_indicator == INDICATOR_DAY_LO)
                time->days = 1;
        }
        break;
    case 2:
        if (is_leap_year((uint16_t)time->years + 2000)) {
            if (time->days > 29) {
                if (setting_page_indicator == INDICATOR_DAY_HI)
                    time->days -= 30;
                else if(setting_page_indicator == INDICATOR_DAY_LO)
                    time->days = 1;
            }
        } else {
            if (time->days > 28) {
                if (setting_page_indicator == INDICATOR_DAY_HI)
                    time->days -= 30;
                else if(setting_page_indicator == INDICATOR_DAY_LO)
                    time->days = 1;
            }
        }
        break;
    default:
        if (time->days > 30)
                time->days = 1;
        break;
    }
}

void __setting_page_adjust(struct time *time)
{
    switch (setting_page_indicator) {
    case INDICATOR_YEAR_HI:
        time->years += 10;
        if (time->years > 99)
            time->years -= 100;
        break;
    case INDICATOR_YEAR_LO:
        time->years += 1;
        if (time->years > 99)
            time->years -= 100;
        break;
    case INDICATOR_MONTH_HI:
        time->months += 10;
        if (time->months > 12)
            time->months -= 20;
        break;
    case INDICATOR_MONTH_LO:
        time->months += 1;
        if (time->months > 12)
            time->months = 1;
        break;
    case INDICATOR_DAY_HI:
        time->days += 10;
        __setting_page_adjust_days(time);
        break;
    case INDICATOR_DAY_LO:
        time->days += 1;
        __setting_page_adjust_days(time);
        break;
    case INDICATOR_HOUR_HI:
        time->hours += 10;
        if (time->hours > 23)
            time->hours -= 30;
        break;
    case INDICATOR_HOUR_LO:
        time->hours += 1;
        if (time->hours > 23)
            time->hours = 0;
        break;
    case INDICATOR_MINUTE_HI:
        time->minutes += 10;
        if (time->minutes >= 60)
            time->minutes = 0;
        break;
    case INDICATOR_MINUTE_LO:
        time->minutes += 1;
        if (time->minutes >= 60)
            time->minutes = 0;
        break;
    default:
        break;
    }
}

void setting_page_init(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;
    struct time *time = &sys_info->time;
    char buffer[32];

    setting_page_indicator = 0;
    epd_init(EPD_MODE_FULL_REFRESH);
    __setting_page_draw(buffer, time);

    epd_turnon();
    clear_all_button_event();
    epd_init(EPD_MODE_PART_REFRESH);
    epd_clear();

    if (sys_info->wakeup_type == WAKEUP_BY_BUTTON)
        set_wakeup_type(WAKEUP_NONE);
}

void setting_page_draw(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;
    struct time *time = &sys_info->time;
    char buffer[32];

    epd_clear();
    __setting_page_draw(buffer, time);
    epd_turnon();
}

int setting_page_ctl(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;
    struct time *time = &sys_info->time;
    unsigned int bt_event;

    while(1)  {
        bt_event = get_button_event(BUTTON2);
        if (bt_event == BT_EVENT_SHORT) {
            setting_page_indicator++;
            return PAGE_KEEP;
        }

        bt_event = get_button_event(BUTTON0);
        if (bt_event == BT_EVENT_SHORT) {
            return PAGE_PREV;
        } else if (bt_event == BT_EVENT_LONG) {
            time->seconds = 0;
            time->weekdays = calc_weekday(time->years + 2000,
                                          time->months,
                                          time->days);
            pcf8563_set_time(time);
            epd_init(EPD_MODE_FULL_REFRESH);
            epd_clear();
            epd_turnon();
            return PAGE_PREV;
        }
        
        bt_event = get_button_event(BUTTON1);
        if (bt_event == BT_EVENT_SHORT) {
            __setting_page_adjust(time);
            return PAGE_KEEP;
        }
    }
    return PAGE_KEEP;
}

void ui_main_loop(void *data)
{
    struct user_page *page = (struct user_page *)&top_pages[0];
    struct sys_info *sys_info = (struct sys_info *)data;
    unsigned int action;

    if (sys_info->wakeup_type == WAKEUP_BY_BUTTON)
        page = page->next;

    while(1) {

page_switch:
        if (page->init)
            page->init(data);

        while(1) {
            page->draw(data);
            action = page->ctl(data);
            if (action == PAGE_BREAK) {
                /* 退出页面循环，休眠 */
                return;
            } else if (action == PAGE_NEXT) {
                page = page->next;
                goto page_switch;
            } else if (action == PAGE_PREV) {
                page = page->prev;
                goto page_switch;
            }
        }
    }
}
