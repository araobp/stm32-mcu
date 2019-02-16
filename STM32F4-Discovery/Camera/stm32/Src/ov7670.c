/*
 * ov7670.c
 *
 *  Created on: 2019/02/12
 */

#include "ov7670.h"

I2C_HandleTypeDef *phi2c;
DCMI_HandleTypeDef *phdcmi;

uint8_t pid;
uint8_t ver;

// OV7670 config data
const uint8_t OV7670_CONFIG[][2] = {
  {COM7_ADDR, QCIF | RGB},
  {COM15_ADDR, ZZ_TO_FF | RGB555},
  //{COM3_ADDR, DCW_ENABLE},
  //{COM14_ADDR, DCW_AND_SCALING_PCLK | ADJUST_MANUALY | DIVIDED_BY_2 },
  {0xFF, 0xFF}
};


/*
const uint8_t OV7670_CONFIG[][2] = {
    {0x3A,0x04},
    {0x12,0x0C},
    {0x8C,0x00},
    {0x04,0x00},
    {0x40,0xD0},
    {0x72,0x11},
    {0x73,0xF1},
    {0xff,0xff}
};
*/
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

void stop_capturing(void) {
  HAL_DCMI_Stop(phdcmi);
  HAL_Delay(30);
}

/**
 * Take a snapshot
 */
void ov7670_take(uint32_t buf_addr, int len) {
  //HAL_StatusTypeDef status;
  //status = HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  //printf("%lu\n", (uint32_t)status);
}

/**
 * OV7670 initialization
 */
void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi) {
  phi2c = p_hi2c;
  phdcmi = p_hdcmi;

  printf("OV7670 init\n");

  // Hardware reset
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);  // GPIO PE1: LOW
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);  // GPIO PE1: HIGH
  HAL_Delay(100);

  // Software reset: reset all registers to default values
  sccb_write(COM7_ADDR, RESET_TO_DEFAULT);
  HAL_Delay(30);

  // Read product ID and version
  sccb_read(PID_ADDR, &pid);  // pid is 0x76
  sccb_read(VER_ADDR, &ver);  // ver is 0x73
  printf("PID: 0x%x, VER: 0x%x\n", pid, ver);

  // Stop capturing
  stop_capturing();
}

/**
 * OV7670 configuration
 */
void ov7670_conf(void) {
  int i = 0;
  uint8_t reg_addr, data, data_read;
  while (1) {
    reg_addr = OV7670_CONFIG[i][0];
    data = OV7670_CONFIG[i][1];
    if (reg_addr == 0xff) break;
    sccb_write(reg_addr, data);
    printf("sccb write: 0x%x 0x%x\n", reg_addr, data);
    HAL_Delay(10);
    sccb_read(reg_addr, &data_read);
    if (data != data_read) {
      printf("sccb write failure: 0x%x 0x%x\n", reg_addr, data_read);
    }
    i++;
  }
}
