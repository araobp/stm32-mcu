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
//----- Common control 3 -----
#define COM3_ADDR 0x0c
// Bit[2]
#define DCW_ENABLE 0b00000100

//----- Common control 7 -----
#define COM7_ADDR 0x12
// Bit[7]
#define RESET_TO_DEFAULT 0b10000000
// Bit[4]
#define QVGA 0b00010000
// Bit[3]
#define QCIF 0b00001000
// Bit[2] and Bit[0]
#define RGB  0b00000100

//----- Common control 14 -----
#define COM14_ADDR 0x3e
// Bit[4]
#define DCW_AND_SCALING_PCLK 0b00010000
// Bit[3]
#define ADJUST_MANUALY 0b00001000
// Bit[2:0]
#define DIVIDED_BY_2 0b00000001

//------ Common Control 15 -----
#define COM15_ADDR 0x40
// Bit[7:6]
#define ZZ_TO_FF 0b11000000
// Bit[5:4]
#define NORMAL   0b00000000
#define RGB565   0b00010000
#define RGB555   0b00110000

//----- Product ID number MSB -----
#define PID_ADDR 0x0a
//----- Product ID number LSB -----
#define VER_ADDR 0x0b

/*----- Picture width and height ------------*/
#define QCIF_WIDTH_PAL  172
#define QCIF_HEIGHT 144

#define QVGA_WIDTH 320
#define QVGA_HEIGHT 240

void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi);

void ov7670_conf(void);

void ov7670_take_snapshot(uint32_t buf_addr, int len);

void ov7670_take_continuous(uint32_t buf_addr, int len);

#endif /* OV7670_H_ */
