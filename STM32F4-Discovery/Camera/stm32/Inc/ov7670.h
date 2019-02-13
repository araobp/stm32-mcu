/*
 * ov7670.h
 *
 *  Created on: 2019/02/12
 */

#ifndef OV7670_H_
#define OV7670_H_

#include "stm32f4xx_hal.h"

/*--- SCCB addresses-------------------------*/
#define SCCB_WRITE_SLAVE_ADDR 0x42
#define SCCB_READ_SLAVE_ADDR 0x43

/*--- Registers -----------------------------*/
#define COM3_ADDR 0x0c
#define DCW_ENABLE 0x40

#define COM7_ADDR 0x12
#define RESET_TO_DEFAULT 0x80
#define QCIF 0b00001000
#define RGB  0b00000100

#define COM3_ADDR 0x0c
#define DCW_ENABLE 0b00000100

#define COM14_ADDR 0x3e
#define DCW_AND_SCALING_PCLK 0b00010000
#define ADJUST_MANUALY 0b00001000
#define DIVIDED_BY_2 0b00000001

#define COM15_ADDR 0x40
#define ZZ_TO_FF 0b11000000
#define RGB565   0b00010000

#define PID_ADDR 0x0a
#define VER_ADDR 0x0b
/*-------------------------------------------*/

const uint8_t OV7670_CONFIG[][2] = {
  {COM7_ADDR, QCIF | RGB},
  {COM15_ADDR, ZZ_TO_FF | RGB565},
  {COM3_ADDR, DCW_ENABLE},
  {COM14_ADDR, DCW_AND_SCALING_PCLK | ADJUST_MANUALY | DIVIDED_BY_2 }
};

void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi);

#endif /* OV7670_H_ */
