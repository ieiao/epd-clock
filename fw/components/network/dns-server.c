#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

static const char *TAG = "dns";
static TaskHandle_t handle_dns_server = NULL;

static void dns_server(void *p)
{
    uint8_t buf[128] = {0};
    int32_t len = 0;
    struct sockaddr_in server = {0};
    struct sockaddr_in client = {0};
    socklen_t client_len = sizeof(struct sockaddr_in);
    int sock;
    int ret;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket.");
        goto end;
    }

    server.sin_family = PF_INET;
    server.sin_port = htons(53);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if (ret < 0) {
        ESP_LOGE(TAG,"Failed to bind socket.");
        goto end;
    }

    while (1)
    {
        len = recvfrom(sock, buf, 100, 0,
                       (struct sockaddr *)&client, &client_len);

        if (len < 0 || len > 100)
            continue;

        buf[2] |= 0x80;     /* response */
        buf[3] |= 0x80;     /* can do recursion */
        buf[7] =1;

        buf[len++] = 0xc0;  /* is a pointer */
        buf[len++] = 0x0c;  /* offset 0x00c */

        buf[len++] = 0x00;
        buf[len++] = 0x01;  /* TYPE A */
        buf[len++] = 0x00;
        buf[len++] = 0x01;  /* class IN */

        buf[len++] = 0x00;
        buf[len++] = 0x00;
        buf[len++] = 0x02;
        buf[len++] = 0x58;  /* valid for 600 seconds */

        buf[len++] = 0x00;
        buf[len++] = 0x04;  /* address 4 bytes long */

        buf[len++] = 192;
        buf[len++] = 168;
        buf[len++] = 4;
        buf[len++] = 1;     /* IP address */

        sendto(sock, buf, len, 0,
               (struct sockaddr*)&client, client_len);
    }
    
end:
    vTaskDelete(NULL);
}

void dns_server_start(void)
{
    xTaskCreate(dns_server, "dns server", 2048, NULL, 5, &handle_dns_server);
}

void dns_server_stop(void)
{
    if (handle_dns_server)
        vTaskDelete(handle_dns_server);
}
