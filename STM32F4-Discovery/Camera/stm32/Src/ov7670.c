/*
 * ov7670.c
 *
 *  Created on: 2019/02/12
 */

#include "ov7670.h"

//#define DEBUG

I2C_HandleTypeDef *phi2c;
DCMI_HandleTypeDef *phdcmi;

uint8_t pid;
uint8_t ver;

/* OV7670 config data
 *
 * Note for my device:
 * - Register at 0xB0 is not in the datasheet. By setting this register,
 * I could get color images (w/o the setting, I got gray scale images).
 * - White balance: Advanced AWB mode is better than Normal AWB mode.
 * - Automatic gain ceiling: 0x4a is default, but 0x6a outputs brighter image.
 * - The following color matrix setting seemed to increase contrast.
 */
const uint8_t OV7670_CONFIG[][2] = {
  // Color matrix setting
#ifdef HIGH_CONTRAST
  {MTX1_ADDR,0x80},
  {MTX2_ADDR,0x80},
  {MTX3_ADDR,0x00},
  {MTX4_ADDR,0x22},
  {MTX5_ADDR,0x5e},
  {MTX6_ADDR,0x80},
  {MTXS_ADDR, 0x9e},
#endif
  // Image size etc
  {COM7_ADDR, QCIF | RGB},
  {COM15_ADDR, ZZ_TO_FF | RGB565},
  // Control features
  {0xB0, 0x84}, // Not in the datasheet, but it is necessary.
  {AWBCTR0_ADDR, 0x93},  // Enable advanced AWB mode
  {COM9_ADDR, 0x4a},  // Automatic Gain ceiling
  {MVFP_ADDR, MIRROR_IMAGE | VERTICALLY_FLIP_IMAGE},
  {COM13_ADDR, GAMMA_ENABLED},
  {0xFF, 0xFF}
};

int sccb_write(uint8_t reg_addr, uint8_t data) {
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

int sccb_read(uint8_t reg_addr, uint8_t *pdata) {
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
void ov7670_take_snapshot(uint32_t buf_addr, int len) {
  //HAL_StatusTypeDef status;
  //status = HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_SNAPSHOT, buf_addr, len);
  //printf("%lu\n", (uint32_t)status);
}

void ov7670_take_continuous(uint32_t buf_addr, int len) {
  HAL_DCMI_Start_DMA(phdcmi, DCMI_MODE_CONTINUOUS, buf_addr, len);
}

/**
 * OV7670 initialization
 */
void ov7670_init(I2C_HandleTypeDef *p_hi2c, DCMI_HandleTypeDef *p_hdcmi) {
  phi2c = p_hi2c;
  phdcmi = p_hdcmi;

#ifdef DEBUG
  printf("OV7670 init\n");
#endif

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
#ifdef DEBUG
  printf("PID: 0x%x, VER: 0x%x\n", pid, ver);
#endif

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

    sccb_read(reg_addr, &data_read);
    sccb_write(reg_addr, data);
#ifdef DEBUG
    printf("sccb write: 0x%x 0x%x=>0x%x\n", reg_addr, data_read, data);
#endif
    HAL_Delay(30);
    sccb_read(reg_addr, &data_read);
#ifdef DEBUG
    if (data != data_read) {
      printf("sccb write failure: 0x%x 0x%x\n", reg_addr, data_read);
    }
#endif
    i++;
  }
}

