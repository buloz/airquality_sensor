//
// Created by buloz on 18/01/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"


#include "wifi_sensor.h"

/* Declaration of event group to signal the WiFi connection */
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

extern const char *TAG;

static esp_err_t wifi_event_handler(void *arg, system_event_t *event)
{
    switch (event->event_id) {
        /* Connection event starting */
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        /* IP addressed */
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        /* SUCCESS WPS connection */
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT);
            break;
        /* IP disconnection */
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
        /* FAILED WPS connection */
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            esp_wifi_connect();
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void wifi_init(void)
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = CONFIG_ESP_WIFI_SSID,
                    .password = CONFIG_ESP_WIFI_PASSWORD,
            },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    //ESP_LOGI(TAG, "start the WIFI SSID:[%s]", CONFIG_ESP_WIFI_SSID);
    printf("start the wifi SSID: [%s]", CONFIG_ESP_WIFI_SSID);
    ESP_ERROR_CHECK(esp_wifi_start());
    printf("waiting for wifi");
    //ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
}
