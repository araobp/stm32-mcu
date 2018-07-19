/*
 * gyro_scope.c
 *
 *  Created on: 2018/07/18
 */

#include <l3gd20.h>

const uint16_t i2c_addr = L3GD20_I2C_ADDRESS << 1;

I2C_HandleTypeDef *phi2c_;

uint8_t init_l3gd20(void) {
  HAL_Delay(100);
  uint8_t status = 0;
  uint8_t who = i2c_read(i2c_addr, WHO_AM_I);
  i2c_write(i2c_addr, CTRL_REG1, ENABLE);
  if (who != RES_OK) status = 1;
  return status;
}

int8_t get_temp(void) {
  int8_t t = i2c_read(i2c_addr, OUT_TEMP);
  return t;
}

float get_motion(char axis) {
  uint8_t msb, lsb;
  // read the registers
  switch(axis) {
    case 'x':
      msb = i2c_read(i2c_addr, OUT_X_H);
      lsb = i2c_read(i2c_addr, OUT_X_L);
      break;
    case 'y':
      msb = i2c_read(i2c_addr, OUT_Y_H);
      lsb = i2c_read(i2c_addr, OUT_Y_L);
      break;
    case 'z':
      msb = i2c_read(i2c_addr, OUT_Z_H);
      lsb = i2c_read(i2c_addr, OUT_Z_L);
      break;
    default:
      msb = 0;
      lsb = 0;
      break;
  }

  // cast to signed int (16bit): 2's complement
  float data = (float)(int16_t)(((msb << 8) & 0xFF00) | lsb);
  data *= SENSITIVITY;  // -250dps to +250dps
  return data;
}

