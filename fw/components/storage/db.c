#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "sys/param.h"
#include "nvs_flash.h"
#include "nvs.h"

#define TAG         "db"

static nvs_handle_t db_handle;

esp_err_t db_get_u32(char *key, uint32_t *out_value)
{
    return nvs_get_u32(db_handle, key, out_value);
}

esp_err_t db_set_u32(char *key, uint32_t value)
{
    return nvs_set_u32(db_handle, key, value);
}

esp_err_t db_get_str(char *key, char *out_value, size_t *length)
{
    return nvs_get_str(db_handle, key, out_value, length);
}

esp_err_t db_set_str(char *key, char *value)
{
    return nvs_set_str(db_handle, key, value);
}

esp_err_t db_init(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
        if (ret != ESP_OK)
            return ret;
    }

    ret = nvs_open("clock", NVS_READWRITE, &db_handle);
    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}

esp_err_t db_commit(void)
{
    return nvs_commit(db_handle);
}

esp_err_t db_erase(void)
{
    return nvs_flash_erase();
}

void db_deinit()
{
    nvs_close(db_handle);
    nvs_flash_deinit();
}
