#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "esp_err.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif_types.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

static const char *TAG = "wifi";

static esp_netif_t *netif_ap = NULL;
static esp_netif_t *netif_sta = NULL;

static int conn_retry_num = 0;
static EventGroupHandle_t wifi_event_group;

#define ESP_MAXIMUM_RETRY       3

/* FreeRTOS event group to signal when we are connected*/
#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAIL_BIT           BIT1

static void net_event_handler(void* arg, esp_event_base_t event_base,
                              int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (conn_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            conn_retry_num++;
            ESP_LOGI(TAG,"connect to the AP fail, retry.");
        } else {
            ESP_LOGI(TAG,"connect to the AP fail, final.");
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        conn_retry_num = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/*
 * return -1: memory alloc failed. 
 * return  x: scaned x ap.
 * */
int wifi_scan(wifi_ap_record_t **wifi_list)
{
    uint16_t ap_count;

    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    *wifi_list = malloc(sizeof(wifi_ap_record_t) * ap_count);
    if (wifi_list == NULL) {
        ESP_LOGE(TAG, "can not alloc memory");
        return ESP_FAIL;
    }
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, *wifi_list));
    return ap_count;
}

esp_err_t sta_start(void)
{
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_t instance_any_id;

    wifi_event_group = xEventGroupCreate();
    esp_netif_init();
    netif_sta = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        net_event_handler,
                                                        NULL,
                                                        &instance_any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        net_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t sta_config = {0};
    strcpy((char *)sta_config.sta.ssid, saved_ap_ssid());
    strcpy((char *)sta_config.sta.password, saved_ap_pwd());

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                            pdFALSE, pdFALSE, portMAX_DELAY);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT,
                ESP_EVENT_ANY_ID, instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT,
                IP_EVENT_STA_GOT_IP, instance_got_ip));
    vEventGroupDelete(wifi_event_group);

    if (bits & WIFI_CONNECTED_BIT)
        return ESP_OK;

    return ESP_FAIL;
}

void sta_stop(void)
{
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_wifi_clear_default_wifi_driver_and_handlers(netif_sta);
    esp_netif_destroy(netif_sta);
}

esp_err_t apsta_start(void)
{
    esp_err_t ret = ESP_OK;
    wifi_config_t ap_config;
    uint8_t buf[6] = {0};

    esp_netif_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK)
        goto bad;

    ret = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    if (ret != ESP_OK)
        goto bad;

    ret = esp_wifi_set_mode(WIFI_MODE_APSTA);
    if (ret != ESP_OK)
        goto bad;

    memset(&ap_config, 0, sizeof(wifi_config_t));
    ap_config.ap.max_connection = 1;
    ap_config.ap.authmode = WIFI_AUTH_OPEN;
    esp_read_mac(buf, ESP_MAC_WIFI_STA);
    sprintf((char *)&ap_config.ap.ssid, "CLOCK-%02X%02X", buf[4], buf[5]);

    netif_ap = esp_netif_create_default_wifi_ap();
    ret = esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    if (ret != ESP_OK)
        goto bad;

    ret = esp_wifi_set_max_tx_power(10);
    ret = esp_wifi_start();
    return ret;

bad:
    return ESP_FAIL;
}

void apsta_stop(void)
{
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_netif_destroy_default_wifi(netif_ap);
}
