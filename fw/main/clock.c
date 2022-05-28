/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.  */
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_netif.h"
#include "esp_sleep.h"
#include "esp_sntp.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "net-manager.h"
#include "hal/adc_types.h"
#include "hal/gpio_types.h"
#include "lwip/apps/sntp.h"
#include "nvs_flash.h"
#include "driver/rtc_io.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "http-server.h"
#include "dns-server.h"
#include "config.h"
#include "epd-2in9.h"
#include "esp_timer.h"
#include "net-manager.h"
#include "weather.h"
#include "clock.h"
#include "display.h"
#include "db.h"
#include "fs.h"

#define TAG     "clock"

RTC_DATA_ATTR static time_t last_sync_time = 0;
RTC_DATA_ATTR static time_t last_sync_weather = 0;
RTC_DATA_ATTR static time_t last_read_battery = 0;
RTC_DATA_ATTR static time_t wakeup_moment = 0;

RTC_DATA_ATTR static struct qweather weather;
RTC_DATA_ATTR static unsigned int voltage;

static struct sysinfo info;

#define WAKEUP_PIN          0
#define BATT_CTL_PIN        20
#define STDBY_PIN           1
#define CHRG_PIN            21

static void clock_gpio_init(void)
{
    gpio_reset_pin(WAKEUP_PIN);
    gpio_set_direction(WAKEUP_PIN, GPIO_MODE_INPUT);

    /* Make sure battery voltage measure is Disabled. */
    gpio_reset_pin(BATT_CTL_PIN);
    gpio_set_direction(BATT_CTL_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BATT_CTL_PIN, GPIO_FLOATING);

    /* Prepare charge detect gpio. */
    gpio_reset_pin(STDBY_PIN);
    gpio_reset_pin(CHRG_PIN);
    gpio_set_direction(STDBY_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(CHRG_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(STDBY_PIN, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(CHRG_PIN, GPIO_PULLUP_ONLY);
}

static void read_battery_voltage(void)
{
    esp_adc_cal_characteristics_t adc_chars;
    uint32_t adc_reading = 0;

    gpio_set_direction(BATT_CTL_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BATT_CTL_PIN, 1);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_0);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 1100, &adc_chars);

    for (int i = 0; i < 8; i++)
        adc_reading += adc1_get_raw(ADC1_CHANNEL_3);

    gpio_set_level(BATT_CTL_PIN, 0);
    gpio_set_direction(BATT_CTL_PIN, GPIO_MODE_INPUT);

    adc_reading /= 8;

    voltage = (unsigned int)(5.7 * esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars));
}

static void obtain_battery_state(void)
{
    if (gpio_get_level(CHRG_PIN) == 1) {
        if (gpio_get_level(STDBY_PIN) == 0)
            info.battery_state = BATT_STDBY;
        else
            info.battery_state = BATT_DISCHARGE;
    } else {
        if (gpio_get_level(STDBY_PIN) == 1)
            info.battery_state = BATT_CHAREING;
        else /* This should never happend*/
            info.battery_state = BATT_MAX;
    }
    gpio_set_pull_mode(STDBY_PIN, GPIO_FLOATING);
    gpio_set_pull_mode(CHRG_PIN, GPIO_FLOATING);
}

static bool night_mode_valid()
{
    struct tm *tm = &info.tm;
    unsigned char night_begin = saved_night_begin();
    unsigned char night_end = saved_night_end();

    if (night_begin == night_end)
        return false;

    if (night_begin > night_end)
        return (tm->tm_hour >= night_begin) ||
               (tm->tm_hour < night_end);
    else
        return (tm->tm_hour >= night_begin) &&
               (tm->tm_hour < night_end);
}

static unsigned int calc_deep_sleep_duration()
{
    time_t *now = &info.now;
    struct tm *tm = &info.tm;
    unsigned int sleep_duration = 60 - tm->tm_sec;

    if (night_mode_valid(tm)) {
        if (info.wakeup_cause == ESP_SLEEP_WAKEUP_GPIO) {
            sleep_duration = 30;
        } else if (wakeup_moment < *now) {
            unsigned char night_begin = saved_night_begin();
            unsigned char night_end = saved_night_end();
            if (night_begin > night_end) {
                if (tm->tm_hour >= night_begin) {
                    wakeup_moment = 24 - tm->tm_hour;
                    wakeup_moment += night_end;
                    wakeup_moment *= 60;
                    wakeup_moment -= tm->tm_min;
                    wakeup_moment *= 60;
                    wakeup_moment -= tm->tm_sec;
                } else {
                    wakeup_moment = 60 * (night_end - tm->tm_hour);
                    wakeup_moment -= tm->tm_min;
                    wakeup_moment *= 60;
                    wakeup_moment -= tm->tm_sec;
                }
            } else {
                wakeup_moment = 60 * (night_end - tm->tm_hour);
                wakeup_moment -= tm->tm_min;
                wakeup_moment *= 60;
                wakeup_moment -= tm->tm_sec;
            }
            wakeup_moment += *now;
        } else {
            sleep_duration = wakeup_moment - *now;
        }
    }

    return sleep_duration;
}

void enable_gpio_wakeup()
{
    esp_deep_sleep_enable_gpio_wakeup(BIT(0), ESP_GPIO_WAKEUP_GPIO_LOW);
}

void enable_timer_wakeup(unsigned int i)
{
    if (i)
        esp_sleep_enable_timer_wakeup((uint64_t)i*1000000);
    else
        esp_sleep_enable_timer_wakeup(
                (uint64_t)calc_deep_sleep_duration()*1000000);
}

/*
 * Use light sleep mode to wait epd flush done.
 * This function should be called immediately after epd refresh cmd send.
 */
static void clock_light_sleep(void)
{
    gpio_wakeup_enable(4, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
    gpio_wakeup_disable(4);
}

static esp_err_t ntp_time_sync(void)
{
    int res = ESP_OK;

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "ntp.ntsc.ac.cn");
    sntp_setservername(1, "pool.ntp.org");
    sntp_init();

    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        vTaskDelay(2000/portTICK_PERIOD_MS);
        res++;
        if (res > 10) {
            res = ESP_FAIL;
            goto err;
        }
    }
    res = ESP_OK;

err:
    sntp_stop();
    return res;
}

static void general_process_timeout(void *arg)
{
    enable_timer_wakeup(30);
    enable_gpio_wakeup();
    esp_deep_sleep_start();
}

const esp_timer_create_args_t general_process_timer = {
    .callback = &general_process_timeout,
    .arg = NULL,
    .name = "general process timer",
};

void general_process(void)
{
    esp_err_t ret;
    esp_timer_handle_t timer;
    bool need_sync_time = false;
    bool need_sync_weather = false;
    bool need_read_battery = false;

    ESP_ERROR_CHECK(esp_timer_create(&general_process_timer, &timer));
    ESP_ERROR_CHECK(esp_timer_start_once(timer, 30000000));

    setenv("TZ", "CST-8", 1);
    tzset();
    time(&info.now);
    localtime_r(&info.now, &info.tm);

    if (info.tm.tm_year < (2016 - 1900) ||
        (info.now - last_sync_time) > 24*60*60) {
        need_sync_time = true;
    }

    if (last_sync_weather == 0 ||
        (info.now - last_sync_weather) > 1*60*60) {
        need_sync_weather = true;
    }

    if (last_read_battery == 0 ||
        (info.now - last_read_battery) > 12*60*60 ||
        info.battery_state == BATT_CHAREING ||
        info.battery_state == BATT_STDBY) {
        need_read_battery = true;
    }

    if (need_read_battery)
        read_battery_voltage();

    /* Never sync time or weather when wakeup by gpio. */
    if ((need_sync_time || need_sync_weather) &&
        info.wakeup_cause != ESP_SLEEP_WAKEUP_GPIO) {
        ret = sta_start();
        if (ret == ESP_OK) {
            if (need_sync_time) {
                ESP_LOGI(TAG, "Getting time over NTP.");
                ret = ntp_time_sync();
                if (ret == ESP_OK) {
                    time(&info.now);
                    localtime_r(&info.now, &info.tm);
                    last_sync_time = info.now;
                } else {
                    info.err = ERR_TIME;
                }
            }

            if (need_sync_weather) {
                ret = qweather_get(info.weather);
                if (ret == ESP_OK)
                    last_sync_weather = info.now;
                else
                    info.err = ERR_WEATHER;
            }
        } else {
            info.err = ERR_WIFI;
        }
        sta_stop();
    }

    if (night_mode_valid() && info.wakeup_cause != ESP_SLEEP_WAKEUP_GPIO)
        display_night_mode();
    else
        display_main(&info);

    clock_light_sleep();
    esp_timer_stop(timer);
}

void config_process(void)
{
    unsigned int count = 0;
    esp_err_t ret = ESP_OK;

    display_config();
    ret = apsta_start();
    if (ret != ESP_OK) {
        // TODO: try to reset?
    }

    dns_server_start();
    ret |= fs_init();
    ret |= http_server_start();
    if (ret != ESP_OK) {
        // TODO: try to reset?
    }

    while(1)
    {
        vTaskDelay(10000/portTICK_PERIOD_MS);
        count++;
        if (count >= 12) {
            /* 2 mins, check any device are connected. */
            wifi_sta_list_t sta_list;
            esp_wifi_ap_get_sta_list(&sta_list);
            if (sta_list.num == 0) {
                http_server_stop();
                fs_deinit();
                dns_server_stop();
                apsta_stop();
                break;
            }
        }
    }
}

void misc_process(void)
{
    display_welcome();
    clock_light_sleep();
    epd_deepsleep();
}

void app_main(void)
{
    esp_event_loop_create_default();

    clock_gpio_init();
    db_init();
    epd2in9_init();

    /* Obtain battery state after nvs and epd init, */
    /* hope gpio state already stable. */
    obtain_battery_state();

    /* Prepare clock info. */
    info.err                = ERR_NONE;
    info.weather            = &weather;
    info.voltage            = &voltage;
    info.wakeup_cause       = esp_sleep_get_wakeup_cause();

    config_init();

    if (config_finished()) {
        /* Everything is good, just run general process. */
        general_process();
        /* Still pressed, switch to config process. */
        if (gpio_get_level(WAKEUP_PIN) == 0)
            goto config_mode;
        epd_deepsleep();
    } else if (info.wakeup_cause == ESP_SLEEP_WAKEUP_GPIO) {
config_mode:
        /* Config not finished and wakeup by gpio, run config process. */
        config_process();
        if (config_finished()) {
            /* Config is finished, just reset everything. */
            db_commit();
            db_deinit();
            esp_restart();
        } else {
            /* Config not finished. */
            misc_process();
            goto enable_button_wakeup;
        }
    } else {
        /* Other case, just refresh display, show logo and necessary info. */
        misc_process();
        goto enable_button_wakeup;
    }

    enable_timer_wakeup(0);
enable_button_wakeup:
    enable_gpio_wakeup();

    db_deinit();
    esp_deep_sleep_start();
}
