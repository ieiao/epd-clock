#ifndef __WEATHER_H__
#define __WEATHER_H__

struct qweather {
    int temp;
    int icon;
};

int qweather_get(struct qweather *weather);

#endif
