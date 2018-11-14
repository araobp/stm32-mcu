#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"

#define LCD_I2C_ADDRESS 0x3e // AQM1602XA-RN-GBW

void write_command(uint8_t command);
void write_data(uint8_t data);
void lcd_init(I2C_HandleTypeDef *phi2c);
void lcd_clear(void);
void lcd_newline(void);
void lcd_move_left(void);
void lcd_move_right(void);
void lcd_string(uint8_t *pbuf, uint8_t len);
void lcd_test(void);
