/*!
 * @file bsec_iot_example.c
 *
 * @brief
 * Example for using of BSEC library in a fixed configuration with the BME680 sensor.
 * This works by running an endless loop in the bsec_iot_loop() function.
 */

/*!
 * @addtogroup bsec_examples BSEC Examples
 * @brief BSEC usage examples
 * @{*/

/**********************************************************************************************************************/
/* header files */
/**********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_sleep.h"
#include "nvs_flash.h"

#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "wifi_sensor.h"
#include "i2c_custom.h"
#include "bsec_integration.h"
#include "http_custom_client.h"

/**********************************************************************************************************************/
/* defines */
/**********************************************************************************************************************/

/*
#define WAKEUP_TIME_US 15000000
#define SENSOR_NAME "airqual01"

#define STATION_IP "192.168.10.12"
#define INFLUX_PORT "8086"
#define INFLUX_TOKEN "i7emevL3uktI5AfQVp-JMnSmN7mDzVdEWje3lJpd5oxp_qPVIfmo5SoVxQlHKzaIw-L5ofXB4xShXsTOiIth0Q=="
*/

RTC_DATA_ATTR int wake_count = 0;

/*!
 * @brief           Posting data to influxDB through HTTP request
 *
 * @param[in]       timestamp       time in nanoseconds
 * @param[in]       iaq             IAQ signal
 * @param[in]       iaq_accuracy    accuracy of IAQ signal
 * @param[in]       temperature     temperature signal
 * @param[in]       humidity        humidity signal
 * @param[in]       pressure        pressure signal
 * @param[in]       raw_temperature raw temperature signal
 * @param[in]       raw_humidity    raw humidity signal
 * @param[in]       gas             raw gas sensor signal
 * @param[in]       bsec_status     value returned by the bsec_do_steps() call
 *
 * @return          none
 */
void post_influx_datas(int64_t timestamp, float iaq, uint8_t iaq_accuracy, float temperature, float humidity,
     float pressure, float raw_temperature, float raw_humidity, float gas, bsec_library_return_t bsec_status)
{
    char data_string[100];
    char url_string[100];
    char token_string[100];
    sprintf(data_string, "measurements,host=%s temperature=%.2f,humidity=%.2f,pressure=%.0f,iaq=%.0f",CONFIG_SENSOR_NAME,temperature,humidity,pressure,iaq);
    sprintf(url_string, "http://%s:%s/api/v2/write?org=test-org&bucket=test-buck&precision=s", CONFIG_STATION_IP, CONFIG_INFLUX_PORT);
    sprintf(token_string, "Token %s", CONFIG_INFLUX_TOKEN);

    printf("url: %s\n", url_string);
    printf("token: %s\n", token_string);
    printf("data: %s\n", data_string);

    http_rest_with_url(url_string, data_string, token_string);
}

void app_main()
{
    return_values_init ret;
    esp_err_t errcode = ESP_OK;
    
    wake_count++;
    nvs_flash_init();

    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)
    {
        printf("Wake up [%d] caused by timer\n", wake_count);
    }
    else
    {
        printf("Wake up [%d] not caused by timer\n", wake_count);
    }
    
    /* I2C master initialization */
    errcode = i2c_init();
    if(errcode != ESP_OK)
    {
        printf("Unable to init i2c. Error code : %s\n", esp_err_to_name(errcode));
    }

    /* Wifi initialization & AP connect */
    errcode = wifi_init();
    if(errcode == ESP_OK)
    {
        printf("connected to ap\nSSID:%s\npassword:%s\n",
                 CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    }
    else if (errcode == ESP_FAIL)
    {
        printf("Failed to connect to\nSSID:%s\npassword:%s\n",
                 CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    }

    /* Initialization of BSEC Software, I2C communication and BME680 sensor measurements */
    ret = bsec_iot_init(BSEC_SAMPLE_RATE_LP, 0.0f, bus_write, bus_read, sleep_funct, state_load, config_load);
    if (ret.bme680_status)
    {
        printf("Could not intialize BME680\n");
    }
    else if (ret.bsec_status)
    {
        printf("Could not intialize BSEC library \n");
    }

    /* Reads BSEC software measurements and use post_influx_data to store them in station database */
    bsec_iot_measure(sleep_funct, get_timestamp_us, post_influx_datas, state_save, 10000);

    printf("*yawn* going to sleep now\n");
    esp_sleep_enable_timer_wakeup(CONFIG_WAKEUP_TIME_US);
    esp_deep_sleep_start();
}

/*! @}*/