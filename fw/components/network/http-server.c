#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_private/wifi.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "net-manager.h"
#include "db.h"
#include "config.h"

static const char *TAG = "httpd";
static httpd_handle_t http_server = NULL;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html")) {
        type = "text/html";
    } else if (CHECK_FILE_EXTENSION(filepath, ".js")) {
        type = "application/javascript";
    } else if (CHECK_FILE_EXTENSION(filepath, ".css")) {
        type = "text/css";
    } else if (CHECK_FILE_EXTENSION(filepath, ".png")) {
        type = "image/png";
    } else if (CHECK_FILE_EXTENSION(filepath, ".ico")) {
        type = "image/x-icon";
    } else if (CHECK_FILE_EXTENSION(filepath, ".svg")) {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}

#define GET_REQ_AP_LIST             "/get/ap-list"
#define GET_REQ_AP_SAVED            "/get/ap-saved"
#define GET_REQ_WEATHER_SAVED       "/get/weather-saved"
#define GET_REQ_NIGHT_SAVED         "/get/night-saved"

static esp_err_t site_handle_get_req(httpd_req_t *req)
{
    if (strcmp(req->uri, GET_REQ_AP_LIST) == 0) {
        wifi_ap_record_t *wifi_list = NULL;
        uint16_t wifi_count, i;

        wifi_count = wifi_scan(&wifi_list);
        for (i = 0; i < wifi_count; i++) {
            httpd_resp_sendstr_chunk(req, (char *)wifi_list[i].ssid);
            if (i < (wifi_count - 1))
                httpd_resp_send_chunk(req, " ", 1);
        }
        httpd_resp_sendstr_chunk(req, NULL);
        free(wifi_list);
        wifi_list = NULL;
        goto success;

    } else if (strcmp(req->uri, GET_REQ_AP_SAVED) == 0) {
        if (ap_already_saved()) {
            httpd_resp_sendstr_chunk(req, saved_ap_ssid());
            httpd_resp_send_chunk(req, " ", 1);
            httpd_resp_sendstr_chunk(req, saved_ap_pwd());
            httpd_resp_sendstr_chunk(req, NULL);
            goto success;
        }
    } else if (strcmp(req->uri, GET_REQ_WEATHER_SAVED) == 0) {
        if (weather_already_saved()) {
            httpd_resp_sendstr_chunk(req, saved_weather_key());
            httpd_resp_send_chunk(req, " ", 1);
            httpd_resp_sendstr_chunk(req, saved_weather_location());
            httpd_resp_sendstr_chunk(req, NULL);
            goto success;
        }
    } else if (strcmp(req->uri, GET_REQ_NIGHT_SAVED) == 0) {
        if (night_already_saved()) {
            char buf[4];
            snprintf(buf, sizeof(buf), "%d", saved_night_begin());
            httpd_resp_sendstr_chunk(req, buf);
            httpd_resp_send_chunk(req, " ", 1);
            snprintf(buf, sizeof(buf), "%d", saved_night_end());
            httpd_resp_sendstr_chunk(req, buf);
            httpd_resp_sendstr_chunk(req, NULL);
            goto success;
        }
    }

    return httpd_resp_send_404(req);
success:
    return ESP_OK;
}

#define SET_REQ_WIFI                "/set/wifi"
#define SET_REQ_WEATHER             "/set/weather"
#define SET_REQ_NIGHT               "/set/night"
#define SET_REQ_EXIT                "/set/exit"
#define SET_REQ_RESET_TO_FACTORY    "/set/reset-to-factory"

static esp_err_t site_handle_set_req(httpd_req_t *req)
{
    esp_err_t ret = ESP_OK;

    if (strncmp(req->uri, SET_REQ_WIFI, strlen(SET_REQ_WIFI)) == 0) {
        /* req->uri: /set/wifi?ssid=xxxx&pwd=xxxx */
        const char *conf = req->uri + strlen(SET_REQ_WIFI)+1;
        char ssid[33], pwd[65];

        memset(ssid, 0, sizeof(ssid));
        memset(pwd, 0, sizeof(pwd));
        ret |= httpd_query_key_value(conf, "ssid", ssid, sizeof(ssid));
        ret |= httpd_query_key_value(conf, "pwd", pwd, sizeof(pwd));

        if (ret == ESP_OK) {
            ret = set_ap_info(ssid, pwd);
            if (ret == ESP_OK)
                goto success;
        }
    } else if (strncmp(req->uri, SET_REQ_WEATHER, strlen(SET_REQ_WEATHER)) == 0) {
        /* req->uri: /set/weather?key=xxxx&location=xxxx */
        const char *conf = req->uri + strlen(SET_REQ_WEATHER)+1;
        char key[33], location[33];

        memset(key, 0, sizeof(key));
        memset(location, 0, sizeof(location));
        ret |= httpd_query_key_value(conf, "key", key, sizeof(key));
        ret |= httpd_query_key_value(conf, "location", location, sizeof(location));

        if (ret == ESP_OK) {
            ret = set_weather_info(key, location);
            if (ret == ESP_OK)
                goto success;
        }
    } else if (strncmp(req->uri, SET_REQ_NIGHT, strlen(SET_REQ_NIGHT)) == 0) {
        /* req->uri: /set/night?begin=xx&end=xx */
        const char *conf = req->uri + strlen(SET_REQ_NIGHT)+1;
        char begin[4], end[4];

        memset(begin, 0, sizeof(begin));
        memset(end, 0, sizeof(end));
        ret |= httpd_query_key_value(conf, "begin", begin, sizeof(begin));
        ret |= httpd_query_key_value(conf, "end", end, sizeof(end));

        if (ret == ESP_OK) {
            ret = set_night_info(begin, end);
            if (ret == ESP_OK)
                goto success;
        }
    } else if (strcmp(req->uri, SET_REQ_EXIT) == 0) {
        httpd_resp_send(req, NULL, 0);
        db_commit();
        db_deinit();
        esp_restart();
    } else if (strcmp(req->uri, SET_REQ_RESET_TO_FACTORY) == 0) {
        if (db_erase() == ESP_OK) {
            httpd_resp_send(req, NULL, 0);
            esp_restart();
        }
    }

    return httpd_resp_send_500(req);
success:
    return httpd_resp_send(req, NULL, 0);
}

static esp_err_t site_basic_handler(httpd_req_t *req)
{
    char buf[512];
    int i;
    FILE *fp;
    esp_err_t res;

    ESP_LOGI(TAG, "uri: %s", req->uri);

    if (strncmp(req->uri, "/get/", 5) == 0)
        return site_handle_get_req(req);
    if (strncmp(req->uri, "/set/", 5) == 0)
        return site_handle_set_req(req);

    strcpy(buf, "/spiffs");
    if (strlen(req->uri) == 1 && strcmp(req->uri, "/") == 0)
        strcat(buf, "/index.html");
    else
        strcat(buf, req->uri);

    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    set_content_type_from_file(req, buf);

    strcat(buf, ".gz");
    fp = fopen(buf, "r");
    if (fp == NULL) {
        set_content_type_from_file(req, "/spiffs/index.html");
        fp = fopen("/spiffs/index.html.gz", "r");
    }

    do {
        i = fread(buf, 1, 512, fp);
        res = httpd_resp_send_chunk(req, buf, i);
    } while (i == 512 && res == ESP_OK);

    httpd_resp_send_chunk(req, NULL, 0);
    fclose(fp);

    return ESP_OK;
}

static const httpd_uri_t site_basic = {
    .uri      = "/*",
    .method   = HTTP_GET,
    .handler  = site_basic_handler,
    .user_ctx = NULL,
};

esp_err_t http_server_start(void)
{
    esp_err_t res;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.uri_match_fn = httpd_uri_match_wildcard;
    res = httpd_start(&http_server, &config);
    if (res != ESP_OK)
        goto out;

    res = httpd_register_uri_handler(http_server, &site_basic);
    ESP_LOGI(TAG, "server started");
out:
    return res;
}

void http_server_stop(void)
{
    httpd_stop(http_server);
    ESP_LOGI(TAG, "server stoped");
}
