#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

int wifi_scan(wifi_ap_record_t **wifi_list);
int sta_start(void);
void sta_stop(void);
esp_err_t apsta_start(void);
void apsta_stop(void);

#ifdef __cplusplus
}
#endif
#endif
