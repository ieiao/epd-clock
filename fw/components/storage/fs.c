#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#define TAG         "spiffs"

static esp_vfs_spiffs_conf_t *conf;

esp_err_t fs_init(void)
{
    conf = malloc(sizeof(esp_vfs_spiffs_conf_t));
    memset(conf, 0, sizeof(esp_vfs_spiffs_conf_t));

    conf->base_path = "/spiffs";
    conf->partition_label = NULL,
    conf->max_files = 10,
    conf->format_if_mount_failed = false;

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
    }

    return ret;
}

void fs_deinit(void)
{
    if (conf)
        esp_vfs_spiffs_unregister(conf->partition_label);
}
