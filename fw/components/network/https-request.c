/*
 * SPDX-FileCopyrightText: The Mbed TLS Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * SPDX-FileContributor: 2015-2022 Espressif Systems (Shanghai) CO LTD
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_tls.h"
#include "sdkconfig.h"
#include "esp_crt_bundle.h"

#include "zlib.h"
#include "zutil.h"
#include "inftrees.h"
#include "inflate.h"

static const char *TAG = "HTTPS";

static const char HTTPS_REQUEST[] = "GET %s HTTP/1.1\r\n"
                             "Host: %s\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

static int __https_get_request(esp_tls_cfg_t cfg, const char *host,
                                const char *url, char *out_buf, size_t out_buf_size)
{
    int ret = 0;

    esp_tls_t *tls = esp_tls_init();
    if (!tls) {
        ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
        ret = -1;
        goto exit;
    }

    if (esp_tls_conn_new_sync(host, strlen(host), 443, &cfg, tls) == 1) {
        ESP_LOGD(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto cleanup;
    }

    int request_bytes = sprintf(out_buf, HTTPS_REQUEST, url, host) + 1;
    int written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls,
                                 out_buf + written_bytes,
                                 request_bytes - written_bytes);
        if (ret >= 0) {
            ESP_LOGD(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            goto cleanup;
        }
    } while (written_bytes < request_bytes);

    ESP_LOGD(TAG, "Reading HTTP response...");
    int readed_bytes = 0;
    memset(out_buf, 0x00, out_buf_size);
    do {
        ret = esp_tls_conn_read(tls, (char *)out_buf + readed_bytes, out_buf_size - readed_bytes);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        } else if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        } else if (ret == 0) {
            ESP_LOGD(TAG, "connection closed");
            break;
        }
        readed_bytes += ret;
    } while (1);
    ret = readed_bytes;

cleanup:
    esp_tls_conn_destroy(tls);

exit:
    return ret;
}

static int network_gzip_decompress(void *in_buf, size_t in_size, void *out_buf, size_t *out_size, size_t out_buf_size)
{
    int err = -1;
    z_stream d_stream = {0}; /* decompression stream */

    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in  = in_buf;
    d_stream.avail_in = 0;
    d_stream.next_out = out_buf;

    err = inflateInit2(&d_stream, 47);
    if (err)
        return err;

    while (d_stream.total_out < out_buf_size-1 && d_stream.total_in < in_size) {
        d_stream.avail_in = d_stream.avail_out = 1;

        err = inflate(&d_stream, Z_NO_FLUSH);
        if(err == Z_STREAM_END)
            break;

        if(err != Z_OK)
            return err;
    }

    err = inflateEnd(&d_stream);
    if (err)
        return err;

    *out_size = d_stream.total_out;
    ((char*)out_buf)[*out_size] = '\0';

    return Z_OK;
}

int https_get_request(const char *host, const char *url,
                      char *out_buf, size_t out_buf_size)
{
    int ret;
    char *http_response = (void*)malloc(out_buf_size);

    esp_tls_cfg_t cfg = {
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    ret = __https_get_request(cfg, host, url, http_response, out_buf_size);
    if (ret < 0)
        goto exit;

    void *resp_body = strstr(http_response, "\r\n\r\n")+strlen("\r\n\r\n");
    size_t resp_length;
    char *length_string = strcasestr(http_response, "Content-Length:");
    if (length_string) {
        sscanf(length_string, "%*s%d", &resp_length);
    } else {
        ESP_LOGE(TAG, "unsupported chunked transfer encoding");
        ret = -1;
        goto exit;
    }

    bool gzip_encoded = false;
    char cotent_encoding_string[16];
    char *content_encoding_line = strcasestr(http_response, "Content-Encoding:");
    if (content_encoding_line) {
        sscanf(content_encoding_line, "%*s%s", cotent_encoding_string);
        if (strcasestr(cotent_encoding_string, "gzip"))
            gzip_encoded = true;
    }

    if (gzip_encoded) {
        size_t out_size = 0;
        ESP_LOGD(TAG, "gzip encoded response, decompressing...");
        ret = network_gzip_decompress(resp_body, resp_length, out_buf, &out_size, out_buf_size);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "gzip data decompression failed, code=%d", ret);
            goto exit;
        }
        ret = out_size;
        ESP_LOGD(TAG, "response size after decompression: %d bytes", out_size);
    } else {
        memcpy(out_buf, resp_body, resp_length);
        ret = resp_length;
    }

exit:
    free(http_response);
    return ret;
}
