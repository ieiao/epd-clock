#include <string.h>
#include <stdint.h>
#include "esp_err.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "config.h"
#include "db.h"

#define TAG     "config"

RTC_DATA_ATTR static uint32_t clock_flag;

#define FLAG_IS_SET(x)      ((clock_flag & (x)) == (x))

#define FLAG_SET_BIT(x)     (clock_flag |= (x))
#define FLAG_CLR_BIT(x)     (clock_flag &= ~(x))

#define KEY_CLOCK_FLAG  "config"

esp_err_t set_clock_flag(unsigned int flag)
{
    FLAG_SET_BIT(flag);
    return db_set_u32(KEY_CLOCK_FLAG, clock_flag);
}

RTC_DATA_ATTR static char wifi_ssid[33];
RTC_DATA_ATTR static char wifi_pwd[65];

#define KEY_AP_SSID     "ssid"
#define KEY_AP_PWD      "pwd"

esp_err_t set_ap_info(char *ssid, char *pwd)
{
    esp_err_t ret = ESP_OK;

    if (strlen(ssid) > 32 || strlen(pwd) > 64) {
        ret = ESP_FAIL;
    } else {
        strncpy(wifi_ssid, ssid, sizeof(wifi_ssid));
        strncpy(wifi_pwd, pwd, sizeof(wifi_pwd));
        ret |= db_set_str(KEY_AP_SSID, ssid);
        ret |= db_set_str(KEY_AP_PWD, pwd);

        if (ret == ESP_OK)
            ret = set_clock_flag(FLAG_WIFI_SAVED);
    }

    return ret;
}

esp_err_t load_ap_info(void)
{
    esp_err_t ret = ESP_OK;
    size_t length = 65;

    ret |= db_get_str(KEY_AP_SSID, wifi_ssid, &length);
    ret |= db_get_str(KEY_AP_PWD, wifi_pwd, &length);

    return ret;
}

bool ap_already_saved(void)
{
    return FLAG_IS_SET(FLAG_WIFI_SAVED);
}

char *saved_ap_ssid(void)
{
    return wifi_ssid;
}

char *saved_ap_pwd(void)
{
    return wifi_pwd;
}

RTC_DATA_ATTR static char weather_key[33];
RTC_DATA_ATTR static char weather_location[33];

#define KEY_WEATHER_KEY         "qwk"
#define KEY_WEATHER_LOCATION    "location"

esp_err_t set_weather_info(char *key, char *location)
{
    esp_err_t ret = ESP_OK;

    if (strlen(key) > 32 || strlen(location) > 32) {
        ret = ESP_FAIL;
    } else {
        strncpy(weather_key, key, sizeof(weather_key));
        strncpy(weather_location, location, sizeof(weather_location));
        ret |= db_set_str(KEY_WEATHER_KEY, key);
        ret |= db_set_str(KEY_WEATHER_LOCATION, location);

        if (ret == ESP_OK)
            ret = set_clock_flag(FLAG_WEATHER_SAVED);
    }

    return ret;
}

esp_err_t load_weather_info(void)
{
    esp_err_t ret = ESP_OK;
    size_t length = 33;

    ret |= db_get_str(KEY_WEATHER_KEY, weather_key, &length);
    ret |= db_get_str(KEY_WEATHER_LOCATION, weather_location, &length);

    return ret;
}

bool weather_already_saved(void)
{
    return FLAG_IS_SET(FLAG_WEATHER_SAVED);
}

char *saved_weather_key(void)
{
    return weather_key;
}

char *saved_weather_location(void)
{
    return weather_location;
}

RTC_DATA_ATTR static unsigned char night_begin;
RTC_DATA_ATTR static unsigned char night_end;

#define KEY_NIGHT_BEGIN         "n_begin"
#define KEY_NIGHT_END           "n_end"

esp_err_t set_night_info(char *begin, char *end)
{
    esp_err_t ret = ESP_OK;

    ret |= db_set_str(KEY_NIGHT_BEGIN, begin);
    ret |= db_set_str(KEY_NIGHT_END, end);
    night_begin = atoi(begin);
    night_end = atoi(end);

    if (ret == ESP_OK)
        ret = set_clock_flag(FLAG_NIGHT_SAVED);

    return ret;
}

esp_err_t load_night_info(void)
{
    esp_err_t ret = ESP_OK;
    size_t length = 8;
    char buf[8];

    ret = db_get_str(KEY_NIGHT_BEGIN, buf, &length);
    if (ret == ESP_OK)
        night_begin = atoi(buf);

    ret = db_get_str(KEY_NIGHT_END, buf, &length);
    if (ret == ESP_OK)
        night_end = atoi(buf);

    return ret;
}

bool night_already_saved(void)
{
    return FLAG_IS_SET(FLAG_NIGHT_SAVED);
}

unsigned char saved_night_begin(void)
{
    return night_begin;
}

unsigned char saved_night_end(void)
{
    return night_end;
}

void config_init(void)
{
    /* Never used or battery reload */
    if (clock_flag == 0) {
        uint32_t v;
        esp_err_t ret;
        ret = db_get_u32(KEY_CLOCK_FLAG, &v);
        if (ret == ESP_OK) {
            clock_flag = v;
            if (FLAG_IS_SET(FLAG_WIFI_SAVED)) {
                /* Load wifi ssid and key. */
                load_ap_info();
            }
            if (FLAG_IS_SET(FLAG_WEATHER_SAVED)) {
                /* Load qweather key and location. */
                load_weather_info();
            }
            if (FLAG_IS_SET(FLAG_NIGHT_SAVED)) {
                /* Load night mode info. */
                load_night_info();
            }
        } else {
            /* very bad, means very fist boot. */
            set_clock_flag(FLAG_NOT_FIRSTBOOT);
        }
    }
}

bool config_finished(void)
{
    if (FLAG_IS_SET(FLAG_NOT_FIRSTBOOT || FLAG_WIFI_SAVED ||
            FLAG_WEATHER_SAVED))
        return true;

    return false;
}
