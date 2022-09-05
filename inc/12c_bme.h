#ifndef I2C_BME_H
#define I2C_BME_H

#include "bme280_defs.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
float stream_sensor_data_forced_mode(struct bme280_dev *dev);
// float get_ambient_temperature(struct bme280_dev *dev);
float get_ambient_temperature();
void connect_bme();
void bme280_driver_close(void);

#endif