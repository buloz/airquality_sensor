
#ifndef __I2C_CUSTOM_H__
#define __I2C_CUSTOM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_err.h"

esp_err_t i2c_init(void);

int8_t bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len);

int8_t bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len);

#endif //__I2C_CUSTOM_H__