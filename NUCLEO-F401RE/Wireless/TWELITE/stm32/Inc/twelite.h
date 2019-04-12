/*
 * twelite.h
 *
 *  Created on: 2019/04/09
 */

#ifndef TWELITE_H_
#define TWELITE_H_

#include <stdint.h>
#include <stdbool.h>
#include "usart.h"

// TWELITE packet
#define DST_NODE 0x00
#define BYTE 0xA0
#define RESPONSE_MSG_DISABLED 0x07
#define ACK_ENABLED 0x01
#define RESEND 0x02
#define TERMINATOR 0xFF
#define EOT 0x04

// The number of retries
#define NUM_RETRY 3U

// Data position in a packet
#define SEQ_NUMBER_POS 6U
#define PAYLOAD_POS 18U
#define EOT_POS 20U

// Delay before reset in case of out-of-sync
#define RESET_DELAY 50U  // 50msec

void twelite_uart_tx(uint8_t *pbuf, uint8_t seq, uint8_t len);
void twelite_uart_rx(uint8_t *pbuf, uint8_t *cmd, uint8_t *seq);

#endif /* TWELITE_H_ */
