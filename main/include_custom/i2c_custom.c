
#include <stdio.h>
#include "driver/i2c.h"

#include "i2c_custom.h"

#define I2C_ACK      0x0
#define I2C_NACK     0x1

#define I2C_BUS      0
#define I2C_SCL      22
#define I2C_SDA      21
#define I2C_FREQ     100000

#define ACK_CHECK_EN 1

esp_err_t i2c_init(void)
{
    esp_err_t ret = ESP_OK;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA;
    conf.scl_io_num = I2C_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_FREQ;
    i2c_param_config(0, &conf);
    ret = i2c_driver_install(0, I2C_MODE_MASTER, 0, 0, 0);
    return ret;
}

/*!
 * @brief           Write operation in either I2C or SPI
 *
 * param[in]        dev_addr        I2C or SPI device address
 * param[in]        reg_addr        register address
 * param[in]        reg_data_ptr    pointer to the data to be written
 * param[in]        data_len        number of bytes to be written
 *
 * @return          result of the bus communication function
 */
int8_t bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr) << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);

    if (reg_addr){
        i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    }

    if (reg_data_ptr){
        i2c_master_write(cmd, reg_data_ptr, data_len, ACK_CHECK_EN);        
    }

    i2c_master_stop(cmd);
    esp_err_t res = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    return res;
}

/*!
 * @brief           Read operation in either I2C or SPI
 *
 * param[in]        dev_addr        I2C or SPI device address
 * param[in]        reg_addr        register address
 * param[out]       reg_data_ptr    pointer to the memory to be used to store the read data
 * param[in]        data_len        number of bytes to be read
 *
 * @return          result of the bus communication function
 */
int8_t bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len)
{
    if (!data_len){
        return 1;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    if (!reg_data_ptr)
        i2c_master_stop(cmd);
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    if (data_len > 1) i2c_master_read(cmd, reg_data_ptr, data_len-1, I2C_ACK);
    i2c_master_read_byte(cmd, reg_data_ptr + data_len-1, I2C_NACK);
    i2c_master_stop(cmd);
    
    esp_err_t res = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return res;
}
