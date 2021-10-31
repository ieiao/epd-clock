#include "ui.h"
#include "button.h"
#include "font.h"
#include "epd-2in9.h"
#include "pcf8563.h"
#include "spi-nor.h"
#include "time.h"
#include "timer.h"
#include "utf8-cov.h"
#include "uart.h"
#include "core.h"
#include <msp430g2553.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include "gpio.h"

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
const char night_string[] = "夜晚模式 ";

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

void __main_page_draw_time(struct sys_info *sys_info)
{
    struct time *time = &sys_info->time;
    uint32_t offset, index;
    char buffer[64];

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

    epd_set_window(8, 48, 23, 247);
    __assemble_date_string(buffer, time);
    draw_utf8_string(buffer, false);

    buffer[0] = ' ';
    buffer[1] = sys_info->bat_val/100 + 0x30;
    buffer[2] = '.';
    buffer[3] = sys_info->bat_val%100/10 + 0x30;
    buffer[4] = sys_info->bat_val%10 + 0x30;
    buffer[5] = '\0';
    draw_utf8_string(buffer, false);
}

void __main_page_draw_night_tip(void)
{
    epd_set_window(0, 0, 127, 295);
    __dot_matrix_copy((uint32_t)NIGHT_FREEZE_TIP_OFFSET, 296*(128/8), false);
}

void main_page_draw(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;

    epd_init(EPD_MODE_FULL_REFRESH);
    epd_clear();

    if (in_night_freeze()) {
        if (sys_info->wakeup_type == WAKEUP_BY_BUTTON) {
            __main_page_draw_time(sys_info);
        } else {
            __main_page_draw_night_tip();
        }
    } else {
        __main_page_draw_time(sys_info);
    }

    epd_turnon();
}

int main_page_ctl(void *data)
{
    if (in_night_freeze()) {
        if (get_button_event(BUTTON2) == BT_EVENT_LONG)
            return PAGE_NEXT;
    } else {
        if (get_button_event(BUTTON0) != BT_EVENT_NONE ||
            get_button_event(BUTTON1) != BT_EVENT_NONE ||
            get_button_event(BUTTON2) != BT_EVENT_NONE)
            return PAGE_NEXT;
    }
    clear_all_button_event();
    return PAGE_BREAK;
}

static int8_t setting_page_indicator;
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
    INDICATOR_NIGHT_BEGIN_HI,
    INDICATOR_NIGHT_BEGIN_LO,
    INDICATOR_NIGHT_END_HI,
    INDICATOR_NIGHT_END_LO,
};

const unsigned char indicator_offset[] = {
    2, 3, 6, 7, 10, 11, 14, 15, 18, 19,
    9, 10, 14, 15,
};

void __setting_page_assemble_indicator(char *buffer, int8_t indicator)
{
    memset(buffer, ' ', indicator_offset[indicator]);
    buffer += indicator_offset[indicator];
    *buffer++ = '^';
    *buffer = '\0';
}

void __assemble_string_night(char *buffer, struct sys_info *sys_info)
{
    strcpy(buffer, hour_string);
    buffer[0] = sys_info->night_begin/10 + 0x30;
    buffer[1] = sys_info->night_begin%10 + 0x30;
    buffer += sizeof(hour_string) - 1;

    *buffer++ = '~';

    strcpy(buffer, hour_string);
    buffer[0] = sys_info->night_end/10 + 0x30;
    buffer[1] = sys_info->night_end%10 + 0x30;
    buffer += sizeof(hour_string) - 1;
}

void __setting_page_draw(char *buffer, struct sys_info *sys_info)
{
    epd_clear();
    epd_set_window(104, 132, 119, 163);
    draw_utf8_string("设置", false);

    /* 时间字符串 */
    epd_set_window(80, 60, 95, 235);
    __assemble_string_date(buffer, &sys_info->time);
    draw_utf8_string(buffer, false);
    __assemble_string_time(buffer, &sys_info->time);
    draw_utf8_string(buffer, false);

    /* 夜间模式字符串 */
    epd_set_window(48, 60, 63, 235);
    draw_utf8_string(night_string, false);
    __assemble_string_night(buffer, sys_info);
    draw_utf8_string(buffer, false);

    if (setting_page_indicator <= INDICATOR_MINUTE_LO) {
        epd_set_window(64, 60, 79, 235);
        __setting_page_assemble_indicator(buffer, setting_page_indicator);
        draw_utf8_string(buffer, false);
        /* 清除掉第二行指示器 */
        epd_set_window(32, 60, 47, 235);
        memset(buffer, ' ', indicator_offset[INDICATOR_NIGHT_END_LO]);
        *(buffer + indicator_offset[INDICATOR_NIGHT_END_LO]) = '\0';
        draw_utf8_string(buffer, false);
    } else {
        /* 清除掉第一行指示器 */
        epd_set_window(64, 60, 79, 235);
        memset(buffer, ' ', indicator_offset[INDICATOR_MINUTE_LO]);
        *(buffer + indicator_offset[INDICATOR_MINUTE_LO]) = '\0';
        draw_utf8_string(buffer, false);
        epd_set_window(32, 60, 47, 235);
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
        if (time->days > 31)
            time->days = 1;
        else if (time->days < 0)
            time->days = 31;
        break;
    case 2:
        if (is_leap_year((uint16_t)time->years + 2000)) {
            if (time->days > 29)
                time->days = 1;
            else if (time->days < 0)
                time->days = 29;
        } else {
            if (time->days > 28)
                time->days = 1;
            else if (time->days < 0)
                time->days = 28;
        }
        break;
    default:
        if (time->days > 30)
            time->days = 1;
        else if (time->days < 0)
            time->days = 30;
        break;
    }
}

void __year_adjust(struct time *time)
{
    if (time->years > 99)
        time->years = 0;
    else if (time->years < 0)
        time->years = 99;
}

void __month_adjust(struct time *time)
{
    if (time->months > 12)
        time->months = 1;
    else if (time->months < 1)
        time->months = 12;
}

void __hour_adjust(struct time *time)
{
    if (time->hours > 23)
        time->hours = 0;
    else if (time->hours < 0)
        time->hours = 23;
}

void __minute_adjust(struct time *time)
{
    if (time->minutes > 59)
        time->minutes = 0;
    else if (time->minutes < 0)
        time->minutes = 59;
}

void __night_begin_adjust(struct sys_info *sys_info)
{
    if (sys_info->night_begin > 23)
        sys_info->night_begin = 0;
    else if (sys_info->night_begin < 0)
        sys_info->night_begin = 23;
}

void __night_end_adjust(struct sys_info *sys_info)
{
    if (sys_info->night_end > 23)
        sys_info->night_end = 0;
    else if (sys_info->night_end < 0)
        sys_info->night_end = 23;
}

void __setting_page_adjust(struct sys_info *sys_info, unsigned int bt_event)
{
    struct time *time = &sys_info->time;

    switch (setting_page_indicator) {
    case INDICATOR_YEAR_HI:
        if (bt_event == BT_EVENT_SHORT)
            time->years += 10;
        else
            time->years -= 10;
        __year_adjust(time);
        break;
    case INDICATOR_YEAR_LO:
        if (bt_event == BT_EVENT_SHORT)
            time->years += 1;
        else
            time->years -= 1;
        __year_adjust(time);
        break;
    case INDICATOR_MONTH_HI:
        if (bt_event == BT_EVENT_SHORT)
            time->months += 10;
        else
            time->months -= 10;
        __month_adjust(time);
        break;
    case INDICATOR_MONTH_LO:
        if (bt_event == BT_EVENT_SHORT)
            time->months += 1;
        else
            time->months -= 1;
        __month_adjust(time);
        break;
    case INDICATOR_DAY_HI:
        if (bt_event == BT_EVENT_SHORT)
            time->days += 10;
        else
            time->days -= 10;
        __setting_page_adjust_days(time);
        break;
    case INDICATOR_DAY_LO:
        if (bt_event == BT_EVENT_SHORT)
            time->days += 1;
        else
            time->days -= 1;
        __setting_page_adjust_days(time);
        break;
    case INDICATOR_HOUR_HI:
        if (bt_event == BT_EVENT_SHORT)
            time->hours += 10;
        else
            time->hours -= 10;
        __hour_adjust(time);
        break;
    case INDICATOR_HOUR_LO:
        if (bt_event == BT_EVENT_SHORT)
            time->hours += 1;
        else
            time->hours -= 1;
        __hour_adjust(time);
        break;
    case INDICATOR_MINUTE_HI:
        if (bt_event == BT_EVENT_SHORT)
            time->minutes += 10;
        else
            time->minutes -= 10;
        __minute_adjust(time);
        break;
    case INDICATOR_MINUTE_LO:
        if (bt_event == BT_EVENT_SHORT)
            time->minutes += 1;
        else
            time->minutes -= 1;
        __minute_adjust(time);
        break;
    case INDICATOR_NIGHT_BEGIN_HI:
        if (bt_event == BT_EVENT_SHORT)
            sys_info->night_begin += 10;
        else
            sys_info->night_begin -= 10;
        __night_begin_adjust(sys_info);
        break;
    case INDICATOR_NIGHT_BEGIN_LO:
        if (bt_event == BT_EVENT_SHORT)
            sys_info->night_begin += 1;
        else
            sys_info->night_begin -= 1;
        __night_begin_adjust(sys_info);
        break;
    case INDICATOR_NIGHT_END_HI:
        if (bt_event == BT_EVENT_SHORT)
            sys_info->night_end += 10;
        else
            sys_info->night_end -= 10;
        __night_end_adjust(sys_info);
        break;
    case INDICATOR_NIGHT_END_LO:
        if (bt_event == BT_EVENT_SHORT)
            sys_info->night_end += 1;
        else
            sys_info->night_end -= 1;
        __night_end_adjust(sys_info);
        break;
    default:
        break;
    }
}

void setting_page_init(void *data)
{
    struct sys_info *sys_info = (struct sys_info *)data;
    char buffer[32];

    setting_page_indicator = 0;
    epd_init(EPD_MODE_FULL_REFRESH);
    __setting_page_draw(buffer, sys_info);

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
    char buffer[32];

    epd_clear();
    __setting_page_draw(buffer, sys_info);
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
            if (setting_page_indicator > INDICATOR_NIGHT_END_LO)
                setting_page_indicator = INDICATOR_YEAR_HI;
            return PAGE_KEEP;
        } else if (bt_event == BT_EVENT_LONG) {
            setting_page_indicator--;
            if (setting_page_indicator < 0)
                setting_page_indicator = INDICATOR_NIGHT_END_LO;
            return PAGE_KEEP;
        }


        bt_event = get_button_event(BUTTON0);
        if (bt_event == BT_EVENT_SHORT) {
            pcf8563_get_time(time);
            return PAGE_PREV;
        } else if (bt_event == BT_EVENT_LONG) {
            time->seconds = 0;
            time->weekdays = calc_weekday(time->years + 2000,
                                          time->months,
                                          time->days);
            pcf8563_set_time(time);
            pcf8563_get_time(time);
            epd_init(EPD_MODE_FULL_REFRESH);
            epd_clear();
            epd_turnon();
            return PAGE_PREV;
        }
        
        bt_event = get_button_event(BUTTON1);
        if (bt_event == BT_EVENT_SHORT || bt_event == BT_EVENT_LONG) {
            __setting_page_adjust(sys_info, bt_event);
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

    if (sys_info->wakeup_type == WAKEUP_BY_BUTTON &&
        !in_night_freeze())
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
