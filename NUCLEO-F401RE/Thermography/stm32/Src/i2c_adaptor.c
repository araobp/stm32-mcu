/*
 * i2c_adaptor.c
 *
 *  Created on: 2018/12/04
 *      Author: shiny
 */

#include "i2c.h"
#include "i2c_adaptor.h"

/*
 * i2c.c
 *
 *  Created on: 2018/07/19
 */

#include "i2c.h"

I2C_HandleTypeDef *phi2c_;

void i2c_adaptor_init(I2C_HandleTypeDef *phi2c) {
  phi2c_ = phi2c;
}

void i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2];
  buf[0] = reg_addr;
  buf[1] = data;
  HAL_I2C_Master_Transmit(phi2c_, i2c_addr, buf, 2, 100);
  HAL_Delay(1);
}

void i2c_buf_write(uint8_t i2c_addr, uint8_t *pbuf, uint8_t len) {
  HAL_I2C_Master_Transmit(phi2c_, i2c_addr, pbuf, len, 100);
  HAL_Delay(1);
}

uint8_t i2c_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t buf[1];
  buf[0] = reg_addr;
  HAL_I2C_Master_Transmit(phi2c_, i2c_addr, buf, 1, 100);
  HAL_I2C_Master_Receive(phi2c_, i2c_addr, buf, 1, 100);
  HAL_Delay(1);
  return buf[0];
}

void i2c_read_registors(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    buffer[i] = i2c_read(i2c_addr, reg_addr++);
  }
}

