/*
 * ov7670.c
 *
 *  Created on: 2019/02/12
 */

#include "ov7670.h"

/*--- SCCB addresses-------------------------*/
#define SCCB_WRITE_SLAVE_ADDR 0x42
#define SCCB_READ_SLAVE_ADDR 0x43

/*--- Registers -----------------------------*/
#define COM3_ADDR 0x0c
#define DCW_ENABLE 0x40

#define COM7_ADDR 0x12
#define RESET_TO_DEFAULT 0x80
#define QCIF 0x08

#define PID_ADDR 0x0a
#define VER_ADDR 0x0b

/*-------------------------------------------*/


I2C_HandleTypeDef *phi2c;
DCMI_HandleTypeDef *phdcmi;

uint8_t pid;
uint8_t ver;

static int sccb_write(uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = { 0 };
  HAL_StatusTypeDef status;

  buf[0] = reg_addr;
  buf[1] = data;
  status = HAL_I2C_Master_Transmit(phi2c, SCCB_WRITE_SLAVE_ADDR, buf, 2, 100);
  if (status == HAL_OK) {
    return 0;
  } else {
    return 1;
  }
}

static int sccb_read(uint8_t reg_addr, uint8_t *pdata) {
  uint8_t buf[1] = { 0 };
  HAL_StatusTypeDef status;

  buf[0] = reg_addr;
  status = HAL_I2C_Master_Transmit(phi2c, SCCB_WRITE_SLAVE_ADDR, buf, 1, 100);
  if (status == HAL_OK) {
    status = HAL_I2C_Master_Receive(phi2c, SCCB_READ_SLAVE_ADDR, pdata, 1, 100);
    if (status == HAL_OK) {
      return 0;
    } else {
      return 2;
    }
  } else {
    return 1;
  }
}

/**
 * OV7670 initialization
 */
void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi) {
  int return_code;
  phi2c = p_hi2c;
  phdcmi = p_hdcmi;

  // Hardware reset
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);  // GPIO PE1: LOW
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);  // GPIO PE1: HIGH
  HAL_Delay(100);

  // Software reset: reset all registers to default values
  sccb_write(COM7_ADDR, RESET_TO_DEFAULT);
  HAL_Delay(30);

  // Read product ID and version
  return_code = sccb_read(PID_ADDR, &pid);  // pid is 0x76
  return_code = sccb_read(VER_ADDR, &ver);  // ver is 0x73

  // Stop capturing
  HAL_DCMI_Stop(phdcmi);
}

