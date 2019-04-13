/*
 * rn4020.c
 *
 *  Created on: 2019/04/14
 */

#include <stdint.h>
#include <stdio.h>
#include <usart.h>
#include "rn4020.h"

void rn4020_uart_tx(uint8_t data) {
  printf("SUW,%s,%02x\n", CHARACTERISTIC_UUID, data);
}
