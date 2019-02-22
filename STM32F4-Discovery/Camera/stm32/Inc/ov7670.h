/*
 * ov7670.h
 *
 *  Created on: 2019/02/12
 */

#ifndef OV7670_H_
#define OV7670_H_

#include "stm32f4xx_hal.h"

/**
 * Image size setting
 * Comment out: QCIF
 * OUTPUT_128: 128x128
 * OUTPUT_32: 32x32
 */
#define OUTPUT_32
//#define OUTPUT_128

/**
 * High-contrast color matrix setting
 */
#define HIGH_CONTRAST

/*--- SCCB addresses-------------------------*/
#define SCCB_WRITE_SLAVE_ADDR 0x42
#define SCCB_READ_SLAVE_ADDR 0x43

/*--- Registers -----------------------------*/

//----- Common control 1 -----
#define COM1_ADDR 0x04

//----- Common control 3 -----
#define COM3_ADDR 0x0c
// Bit[2]
#define DCW_ENABLE 0b00000100
#define ENABLE_SCALING 0b00001000

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
#define PROCESSED_BAYER_RAW  0b00000101

//----- Common control 9
#define COM9_ADDR 0x14

//----- Common control 13
#define COM13_ADDR 0x3d
#define GAMMA_ENABLED 0b10000000
#define UYVY 0b00000100

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

//----- CLKRC -----
#define CLKRC_ADDR 0x80

//----- White balance -----
#define AWBCTR0_ADDR 0x6f

//----- TSLB -----
#define TSLB_ADDR 0x0c

//---- MTX1 ~ MTX6 -----
#define MTX1_ADDR 0x4f
#define MTX2_ADDR 0x50
#define MTX3_ADDR 0x51
#define MTX4_ADDR 0x52
#define MTX5_ADDR 0x53
#define MTX6_ADDR 0x54
#define MTXS_ADDR 0x58

//----- Mirror/Vflip -----
#define MVFP_ADDR 0x1e
#define MIRROR_IMAGE 0b00100000
#define VERTICALLY_FLIP_IMAGE 0b00010000

//----- Contrast/brightness -----
#define BRIGHT_ADDR 0x55
#define CONTRAS_ADDR 0x56

//----- Product ID number MSB -----
#define PID_ADDR 0x0a
//----- Product ID number LSB -----
#define VER_ADDR 0x0b

/*----- Picture width and height ------------*/
#define QCIF_WIDTH 172
#define QCIF_HEIGHT 144

#define QVGA_WIDTH 320
#define QVGA_HEIGHT 240


int sccb_read(uint8_t reg_addr, uint8_t *pdata);

int sccb_write(uint8_t reg_addr, uint8_t data);

void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi);

void ov7670_conf(void);

void ov7670_take_snapshot(uint32_t buf_addr, int len);

void ov7670_take_continuous(uint32_t buf_addr, int len);

#endif /* OV7670_H_ */
