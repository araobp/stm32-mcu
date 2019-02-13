/*
 * ov7670.h
 *
 *  Created on: 2019/02/12
 */

#ifndef OV7670_H_
#define OV7670_H_

#include "stm32f4xx_hal.h"

void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi);

#endif /* OV7670_H_ */
