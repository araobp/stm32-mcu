/*
 * rn4020.c
 *
 *  Created on: 2019/04/25
 */

#include "rn4020.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "app_nfc_uri.h"

#define BUFSIZE 64U

uint8_t uart_rx_data;
uint8_t data_buf[BUFSIZE/2];
volatile bool command_received = false;
volatile int data_len;
uint8_t cnt = 0;

char send_buf[100];

typedef enum {
  CNT_CHARA, URI_CHARA
} tx_chara;


/**
 * Transfer byte array to a BLE central via RN4020 module.
 *
 * Note: binary data (uint8_t) is converted into hex in ASCII,
 * because RN4020 accepts binary data in ASCII mode.
 *
 */
void notify(uint8_t *data, int len) {

  int i = 0;
  int idx = 0;
  char ascii_hex_buf[3];

  // TX characteristics
  strcpy(send_buf, NOTIFY_CHARA);

  while (true) {
    sprintf(ascii_hex_buf, "%02x", data[idx]);
    send_buf[37+i*2] = ascii_hex_buf[0];
    send_buf[37+i*2+1] = ascii_hex_buf[1];
    i++;
    idx++;
    if (i == 20) {  // The length of 20 bytes
      send_buf[37 + i*2] = '\n';
      HAL_UART_Transmit(&huart6, (uint8_t *)send_buf, 37+i*2+1, 0xffff);
      // For debug
      send_buf[37 + i*2] = '\0';
      printf("sendData(@20): %s\n", send_buf);
      // NOTIFY interval
      HAL_Delay(500);
      i = 0;
    } else if (idx >= len) {  // The length of data
      send_buf[37 + i*2] = '\n';
      HAL_UART_Transmit(&huart6, (uint8_t *)send_buf, 37+i*2+1, 0xffff);
      // For debug
      send_buf[37 + i*2] = '\0';
      printf("sendData(@len): %s\n", send_buf);
      break;
    }
  }
}
