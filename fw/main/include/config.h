#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

enum {
    FLAG_WIFI_SAVED_SHIFT = 0,
    FLAG_WEATHER_SAVED_SHIFT,
    FLAG_NIGHT_SAVED_SHIFT,
    FLAG_NOT_FIRSTBOOT_SHIFT = 31
};

#define FLAG_WIFI_SAVED         (1 << FLAG_WIFI_SAVED_SHIFT)
#define FLAG_WEATHER_SAVED      (1 << FLAG_WEATHER_SAVED_SHIFT)
#define FLAG_NIGHT_SAVED        (1 << FLAG_NIGHT_SAVED_SHIFT)
#define FLAG_NOT_FIRSTBOOT      (1 << FLAG_NOT_FIRSTBOOT_SHIFT)

esp_err_t set_ap_info(char *ssid, char *pwd);
bool ap_already_saved(void);
char *saved_ap_ssid(void);
char *saved_ap_pwd(void);

esp_err_t set_weather_info(char *key, char *location);
bool weather_already_saved(void);
char *saved_weather_key(void);
char *saved_weather_location(void);

esp_err_t set_night_info(char *begin, char *end);
bool night_already_saved(void);
unsigned char saved_night_begin(void);
unsigned char saved_night_end(void);

void config_init(void);
bool config_finished(void);

#ifdef __cplusplus
}
#endif

#endif
