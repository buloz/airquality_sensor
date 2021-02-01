#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

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

#define WAKEUP_TIME_US 5000000

RTC_DATA_ATTR int wake_count = 0;

void app_main(void)
{
    wake_count++;
    nvs_flash_init();

    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)
    {
        printf("Wake up [%d] caused by timer\n", wake_count);
        //Conecting to CONFIG_ESP_WIFI_SSID AP
        wifi_init();
    }
    else
    {
        printf("Wake up [%d] not caused by timer\n", wake_count);
    }

    /* Entering deep sleep */
    printf("*yawn* going to sleep now\n");
    esp_sleep_enable_timer_wakeup(WAKEUP_TIME_US);
    esp_deep_sleep_start();
}
