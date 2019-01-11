/*
 * amg8833.c
 *
 *  Created on: 2018/12/04
 *      Author: shiny
 */

#include <amg8833.h>

I2C_HandleTypeDef *phi2c_;

void adaptor_init(I2C_HandleTypeDef *phi2c) {
  phi2c_ = phi2c;
}

void i2c_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2];
  buf[0] = reg_addr;
  buf[1] = data;
  HAL_I2C_Master_Transmit(phi2c_, i2c_addr, buf, 2, 100);
  HAL_Delay(1);
}

void set_moving_average(bool enable) {
  uint8_t reg_addr_sequence[5] = {AMG8833_1F_ADDR, AMG8833_1F_ADDR, AMG8833_1F_ADDR,
      AMG8833_AVE_ADDR, AMG8833_1F_ADDR};
  uint8_t enable_sequence[5] = {0x50, 0x45, 0x57, 0x20, 0x00};
  uint8_t disable_sequence[5] = {0x50, 0x45, 0x57, 0x00, 0x00};
  uint8_t *pSeq;

  if (enable) {
    pSeq = enable_sequence;
  } else {
    pSeq = disable_sequence;
  }

  for (int i = 0; i < sizeof(reg_addr_sequence); i++) {
    i2c_write(AMG8833_DEV_ADDR, reg_addr_sequence[i], *(pSeq+i));
  }
}

void read_registors(uint8_t reg_addr, uint8_t *buffer, uint8_t length) {
  uint8_t buf[1];
  buf[0] = reg_addr;
  HAL_I2C_Master_Transmit(phi2c_, AMG8833_DEV_ADDR, buf, 1, 100);
  HAL_I2C_Master_Receive(phi2c_, AMG8833_DEV_ADDR, buffer, length, 100);
}
