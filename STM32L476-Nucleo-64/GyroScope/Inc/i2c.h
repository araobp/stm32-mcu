/*
 * i2c.c
 *
 *  Created on: 2018/07/19
 */

#ifndef I2C_C_
#define I2C_C_

#include <stdint.h>
#include <stm32l4xx_hal.h>

void i2c_init(I2C_HandleTypeDef *phi2c);
void i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_read(uint8_t i2c_addr, uint8_t reg_addr);

#endif /* I2C_C_ */
