#ifndef __DB_H__
#define __DB_H__

#include "esp_err.h"

esp_err_t db_get_u32(char *key, uint32_t *out_value);
esp_err_t db_set_u32(char *key, uint32_t value);

esp_err_t db_get_str(char *key, char *out_value, size_t *length);
esp_err_t db_set_str(char *key, char *value);

esp_err_t db_init(void);
esp_err_t db_erase(void);
esp_err_t db_commit(void);
void db_deinit();

#endif
