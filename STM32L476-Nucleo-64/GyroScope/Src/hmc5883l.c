/*
 * hmc5883l.c
 *
 *  Created on: 2018/07/20
 */

#include <math.h>
#include "i2c.h"
#include "hmc5883l.h"

// HMC5883L
// Reference: https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf

// HMC5883L device address (7bit)
const uint8_t HMC5883L = 0x1E << 1;

// command sequence
uint8_t CRA_00[2] = {0x00, 0x70};
uint8_t CRB_01[2] = {0x01, 0xa0};
uint8_t MODE_02[2] = {0x02, 0x00};
uint8_t READ_ALL_SIX_BYTES[2] = {0x3d, 0x06};
uint8_t POINT_TO_FIRST_REGISTER[2] = {0x3c, 0x03};

// constants for calibration
float Xsf=1.0;
float Ysf=1.0;
float Xoff=0.0;
float Yoff=0.0;

// set X-axis/Y-axix scale factor
void set_constants(int16_t x_max, int16_t x_min, int16_t y_max, int16_t y_min);

// initialize the device
void init_hmc5883l(void) {
  set_constants(X_MAX, X_MIN, Y_MAX, Y_MIN);
  i2c_buf_write(HMC5883L, CRA_00, 2);
  i2c_buf_write(HMC5883L, CRB_01, 2);
  i2c_buf_write(HMC5883L, MODE_02, 2);
  HAL_Delay(6);
}

// get direction in radian
double get_radian(void)
{
  uint8_t data[6];
  uint8_t x_msb;
  uint8_t x_lsb;
  uint8_t y_msb;
  uint8_t y_lsb;

  // read the registers
  i2c_buf_read(HMC5883L, READ_ALL_SIX_BYTES, 2, data, 6);

  x_msb = data[0];
  x_lsb = data[1];
  y_msb = data[4];
  y_lsb = data[5];

  // cast to signed int (16bit): 2's complement
  int16_t x = (int16_t)(((x_msb << 8) & 0xFF00) | x_lsb);
  int16_t y = (int16_t)(((y_msb << 8) & 0xFF00) | y_lsb);
  //int16_t z = (int16_t)(((z_msb << 8) & 0xFF00) | z_lsb);

  // calibration (assuming that the sensor is level with the ground),
  // i.e., XH is x and YH is y.
  x = Xsf * x + Xoff;
  y = Ysf * y + Yoff;

  i2c_buf_write(HMC5883L, POINT_TO_FIRST_REGISTER, 2);
  HAL_Delay(67);

  // return radian
  return atan2((double)y, (double)x);
}

// get direction in degree
int16_t get_degree(void) {
  double rad = get_radian();
  return (int16_t)(rad * 180.0 / M_PI);
}

void set_constants(int16_t x_max, int16_t x_min, int16_t y_max, int16_t y_min) {
  int16_t x_sf = (y_max - y_min) / (x_max - x_min);
  if (x_sf > 1.0) {
    Xsf = x_sf;
  }
  int16_t y_sf = (x_max - x_min) / (y_max - y_min);
  if (y_sf > 1.0) {
    Ysf = x_sf;
  }
  Xoff=((x_max - x_min) / 2 - x_max) * Xsf;
  Yoff=((y_max - y_min) / 2 - y_max) * Ysf;
}



