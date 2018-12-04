/*
 * i2c_adaptor.h
 *
 *  Created on: 2018/12/04
 *      Author: shiny
 */

#ifndef I2C_ADAPTOR_H_
#define I2C_ADAPTOR_H_

#define AMG8833_DEV_ADDR 0x68U << 1
#define AMG8833_PIXEL_DATA_HEAD 0x80U
#define AMG8833_PIXEL_DATA_LENGTH 128U
#define AMG8833_PIXEL_DATA_LENGTH_16bit 64U

void i2c_adaptor_init(I2C_HandleTypeDef *phi2c);
void i2c_read_registors(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length);
void i2c_read_registors_16bit(uint8_t i2c_addr, uint8_t reg_addr, uint16_t *buffer, uint8_t length);

#endif /* I2C_ADAPTOR_H_ */
