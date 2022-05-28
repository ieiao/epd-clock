#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "esp_log.h"
#include "epd-2in9.h"
#include "font.h"
#include "clock.h"

#define TAG             "display"

#define NIGHT_MOD_STR   "Night Mode"
#define WELCOME_STR     "Press button to config"
#define CONFIG_STR      "Config mode is enable"

const char err_str[ERR_MAX][16] = {
    {"None"},
    {"WiFi"},
    {"NTP"},
    {"Weather"}
};

const char battery_str[BATT_MAX][16] = {
    {"Discharging"},
    {"Charging"},
    {"Standby"},
};

static void __fill_datetime(struct sysinfo *info)
{
    int i = 0;
    char buf[64];
    struct tm *tm = &info->tm;

    strftime(buf, sizeof(buf), "%Y/%m/%d %a", tm);
    while(buf[i] != 0){
        epd_area_fill(1, 8*(i+1), 8, 2, font_offset(ascii, buf[i]));
        i++;
    }

    epd_area_fill(3, 0,   50, 10, font_offset(time, tm->tm_hour/10));
    epd_area_fill(3, 50,  50, 10, font_offset(time, tm->tm_hour%10));
    epd_area_fill(3, 100, 14, 10, font_offset(time, 10));
    epd_area_fill(3, 114, 50, 10, font_offset(time, tm->tm_min/10));
    epd_area_fill(3, 164, 50, 10, font_offset(time, tm->tm_min%10));
}

static void __fill_bar(void)
{
    epd_area_fill(2, 216, 3, 12, (char *)image_bar);
}

static void __fill_weather(struct sysinfo *info)
{
    struct qweather *weather = info->weather;
    char *icon;
    char buf[8];
    int i = 0;
    int c;

    c = snprintf(buf, sizeof(buf), "%d", abs(weather->temp));
    c++;
    if (weather->temp < 0)
        c++;
    c = 219 + (77-19*c)/2;

    if (weather->temp < 0) {
        epd_area_fill(2, c, 19, 5, font_offset(temp, 11));
        c += 19;
    }

    while(buf[i]) {
        epd_area_fill(2, c, 19, 5, font_offset(temp, buf[i]-0x30));
        i += 1;
        c += 19;
    }
    epd_area_fill(2, c, 19, 5, font_offset(temp, 10));

    switch (weather->icon) {
    case 100:
    case 150:
        icon = (char *)icon_sunny;
        break;

    case 101 ... 103:
    case 151 ... 153:
        icon = (char *)icon_sunny_cloudy;
        break;

    case 104:
    case 154:
        icon = (char *)icon_cloudy;
        break;

    case 300 ... 399:
        icon = (char *)icon_ranning;
        break;

    case 400 ... 499:
        icon = (char *)icon_snow;
        break;

    case 500 ... 515:
        icon = (char *)icon_fog_smog;
        break;

    default:
        icon = (char *)icon_sunny;
        break;
    }

    epd_area_fill(7, 231, 56, 7, icon);
}

static void __fill_error_code(int err)
{
    char buf[16];
    int i = 0;

    if (err) {
        snprintf(buf, sizeof(buf), "E:%s", err_str[err]);
        while(buf[i] != 0){
            epd_area_fill(13, 8*(i+1), 8, 2, font_offset(ascii, buf[i]));
            i++;
        }
    }
}

static void __fill_battery_info(unsigned int voltage, unsigned int state)
{
    char buf[16];
    int i = 0;

    buf[i++] = voltage/1000 + 0x30;
    buf[i++] = '.';
    buf[i++] = (voltage%1000)/100 + 0x30;
    buf[i++] = (voltage%100)/10 + 0x30;
    buf[i++] = 0x0;

    i = 0;
    while(buf[i] != 0){
        epd_area_fill(1, 176 + 8*i, 8, 2, font_offset(ascii, buf[i]));
        i++;
    }

    if (state != BATT_DISCHARGE && state != BATT_MAX) {
        char *p = (char *)battery_str[state];
        int x = 216 - (strlen(p) + 1) * 8;
        i = 0;
        while(battery_str[state][i] != 0) {
            epd_area_fill(13, x+8*i, 8, 2,
                    font_offset(ascii, battery_str[state][i]));
            i++;
        }
    }
}

void display_main(struct sysinfo *info)
{
    epd_clear(0xff);
    __fill_datetime(info);
    __fill_bar();
    __fill_weather(info);
    __fill_error_code(info->err);
    __fill_battery_info(*info->voltage, info->battery_state);
    epd_display_update();
}

void display_night_mode(void)
{
    char *str = NIGHT_MOD_STR;
    int i = 0;

    epd_clear(0xff);
    while(str[i] != 0) {
        epd_area_fill(7, 108 + 8*i, 8, 2, font_offset(ascii, str[i]));
        i++;
    }

    epd_display_update();
}

void display_welcome(void)
{
    char *str = WELCOME_STR;
    int i = 0;

    epd_clear(0xff);
    while(str[i] != 0) {
        epd_area_fill(7, 64 + 8*i, 8, 2, font_offset(ascii, str[i]));
        i++;
    }

    epd_display_update();
}

void display_config(void)
{
    char *str = CONFIG_STR;
    int i = 0;

    epd_clear(0xff);
    while(str[i] != 0) {
        epd_area_fill(7, 68 + 8*i, 8, 2, font_offset(ascii, str[i]));
        i++;
    }

    epd_display_update();
}
