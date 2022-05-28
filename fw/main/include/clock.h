#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <time.h>
#include "weather.h"
#include "esp_sleep.h"

struct sysinfo {
    esp_sleep_wakeup_cause_t wakeup_cause;
    int err;

    unsigned int battery_state;
    unsigned int *voltage;

    time_t now;
    struct tm tm;
    struct qweather *weather;
};

enum {
    ERR_NONE = 0,
    ERR_WIFI,
    ERR_TIME,
    ERR_WEATHER,
    ERR_MAX
};

enum {
    BATT_DISCHARGE = 0,
    BATT_CHAREING,
    BATT_STDBY,
    BATT_MAX
};

#endif
