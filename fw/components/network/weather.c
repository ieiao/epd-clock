#include "include/https-request.h"
#include "include/weather.h"
#include "config.h"

#include <cJSON.h>
#include <stdio.h>
#include <string.h>

#define QWEATHER_HOST   "devapi.qweather.com"
#define QWEATHER_REQ    "/v7/weather/now?location=%s&key=%s"

int qweather_get(struct qweather *weather)
{
    int ret;
    char *req  = (void *)malloc(256);
    char *resp = (void *)malloc(1024);

    sprintf(req, QWEATHER_REQ, saved_weather_location(), saved_weather_key());
    ret = https_get_request(QWEATHER_HOST, req, resp, 1024);
    if (ret < 0)
        goto err1;

    cJSON *root = cJSON_Parse(resp);
    if (root == NULL) {
        ret = -1;
        goto err1;
    }

    cJSON *now = cJSON_GetObjectItem(root, "now");
    if (now == NULL) {
        ret = -1;
        goto err2;
    }

    cJSON *temp = cJSON_GetObjectItem(now, "temp");
    if (temp == NULL) {
        ret = -1;
        goto err2;
    }

    cJSON *icon = cJSON_GetObjectItem(now, "icon");
    if (icon == NULL) {
        ret = -1;
        goto err2;
    }

    weather->temp = atoi(cJSON_GetStringValue(temp));
    weather->icon = atoi(cJSON_GetStringValue(icon));
    ret = 0;

err2:
    cJSON_Delete(root);
err1:
    free(req);
    free(resp);
    return ret;
}
