#include "bme_dependency.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**********************************************************************************************************************/
/* Dependencies for functionnality of bsec_integration */
/**********************************************************************************************************************/

/*!
 * @brief           System specific implementation of sleep function
 *
 * @param[in]       t_ms    time in milliseconds
 *
 * @return          none
 */
void sleep_funct(uint32_t t_ms)
{
    vTaskDelay(t_ms / portTICK_RATE_MS);
}

/*!
 * @brief           Capture the system time in microseconds
 *
 * @return          system_current_time    current system timestamp in microseconds
 */
int64_t get_timestamp_us()
{
    struct timeval time;
    gettimeofday(&time,0);
    return time.tv_sec*1e6 + time.tv_usec;
}

/*!
 * @brief           Handling of the ready outputs
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
void output_ready(int64_t timestamp, float iaq, uint8_t iaq_accuracy, float temperature, float humidity,
     float pressure, float raw_temperature, float raw_humidity, float gas, bsec_library_return_t bsec_status)
{
    printf("temp: %.2f | hum: %.0f | pres: %.0f | iaq: %.0f \n",temperature,humidity,pressure,iaq);
}

/*!
 * @brief           Load previous library state from non-volatile memory
 *
 * @param[in,out]   state_buffer    buffer to hold the loaded state string
 * @param[in]       n_buffer        size of the allocated state buffer
 *
 * @return          number of bytes copied to state_buffer
 */
uint32_t state_load(uint8_t *state_buffer, uint32_t n_buffer)
{
    // ...
    // Load a previous library state from non-volatile memory, if available.
    //
    // Return zero if loading was unsuccessful or no state was available, 
    // otherwise return length of loaded state string.
    // ...
    return 0;
}

/*!
 * @brief           Save library state to non-volatile memory
 *
 * @param[in]       state_buffer    buffer holding the state to be stored
 * @param[in]       length          length of the state string to be stored
 *
 * @return          none
 */
void state_save(const uint8_t *state_buffer, uint32_t length)
{
    // ...
    // Save the string some form of non-volatile memory, if possible.
    // ...
}
 
/*!
 * @brief           Load library config from non-volatile memory
 *
 * @param[in,out]   config_buffer    buffer to hold the loaded state string
 * @param[in]       n_buffer        size of the allocated state buffer
 *
 * @return          number of bytes copied to config_buffer
 */
uint32_t config_load(uint8_t *config_buffer, uint32_t n_buffer)
{
    // ...
    // Load a library config from non-volatile memory, if available.
    //
    // Return zero if loading was unsuccessful or no config was available, 
    // otherwise return length of loaded config string.
    // ...
    return 0;
}
