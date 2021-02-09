#include <stdio.h>
#include <sys/time.h>
#include "bsec_integration.h"


void sleep_funct(uint32_t t_ms);

int64_t get_timestamp_us();

void output_ready(int64_t timestamp, float iaq, uint8_t iaq_accuracy, float temperature, float humidity,
     float pressure, float raw_temperature, float raw_humidity, float gas, bsec_library_return_t bsec_status);

uint32_t state_load(uint8_t *state_buffer, uint32_t n_buffer);

void state_save(const uint8_t *state_buffer, uint32_t length);

uint32_t config_load(uint8_t *config_buffer, uint32_t n_buffer);
