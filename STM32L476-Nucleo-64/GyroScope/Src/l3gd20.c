/*
 * gyro_scope.c
 *
 *  Created on: 2018/07/18
 */

#include "gyro_scope.h"

const uint16_t i2c_addr_write = L3GD20_I2C_ADDRESS << 1;
const uint16_t i2c_addr_read = ( L3GD20_I2C_ADDRESS << 1 ) + 1;

I2C_HandleTypeDef *phi2c;

void write_data(uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2];
  buf[0] = reg_addr;
  buf[1] = data;
  HAL_I2C_Master_Transmit(phi2c, i2c_addr_write, buf, 2, 100);
  HAL_Delay(1);
}

uint8_t read_data(uint8_t reg_addr) {
  uint8_t buf[1];
  buf[0] = reg_addr;
  HAL_I2C_Master_Transmit(phi2c, i2c_addr_write, buf, 1, 100);
  HAL_I2C_Master_Receive(phi2c, i2c_addr_read, buf, 1, 100);
  HAL_Delay(1);
  return buf[0];
}

uint8_t init_l3gd20(void) {
  uint8_t status;
  uint8_t who = read_data(WHO_AM_I);
  if (who == RES_OK) {
    status = 0;
  } else {
    status = 1;
  }
  write_data(CTRL_REG1, ENABLE);
  return status;
}

int8_t get_temp(void) {
  int8_t t = read_data(OUT_TEMP);
  return t;
}

int16_t get_motion(char axis) {
  uint8_t msb, lsb;
  // read the registers
  switch(axis) {
    case 'x':
      msb = read_data(OUT_X_H);
      lsb = read_data(OUT_X_L);
      break;
    case 'y':
      msb = read_data(OUT_Y_H);
      lsb = read_data(OUT_Y_L);
      break;
    case 'z':
      msb = read_data(OUT_Z_H);
      lsb = read_data(OUT_Z_L);
      break;
    default:
      msb = 0;
      lsb = 0;
      break;
  }

  // cast to signed int (16bit): 2's complement
  int16_t data = (int16_t)(((msb << 8) & 0xFF00) | lsb);
  data *= SENSIVITY;  // -250dps to +250dps
  return data;
}

