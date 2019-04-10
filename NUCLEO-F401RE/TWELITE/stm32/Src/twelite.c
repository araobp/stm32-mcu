/*
 * twelite.c
 *
 *  Created on: 2019/04/09
 */

#include "twelite.h"
#include <stdint.h>
#include <stdio.h>

void send_one(uint8_t data) {
  HAL_UART_Transmit(&huart1, &data, 1, 0xFFFFFFFF);
}

void twelite_uart_tx(uint8_t *pbuf, uint8_t seq, uint8_t len) {
    static uint8_t cs;

    //--- Binary transfer mode header
    //printf("start\n");
    send_one(0xA5); // Binary transfer mode header
    send_one(0x5A); // Binary transfer mode header
    send_one(0x80); // Data length MSB
    send_one(len + 8); // Data length LSB
    //putchar(len+5);  // Data length LSB
    //--- Packet header
    send_one(DST_NODE); // Destination is "parent node"
    send_one(BYTE); // Byte (fixed)
    send_one(seq); // Sequence number
    //--- Options
    send_one(RESPONSE_MSG_DISABLED);
    send_one(ACK_ENABLED); // ACK enabled
    send_one(RESEND); // Resend
    send_one(NUM_RETRY); // The number of retries
    send_one(TERMINATOR); // Terminator
    //--- Payload
    cs = DST_NODE ^ BYTE ^ seq ^ RESPONSE_MSG_DISABLED ^ ACK_ENABLED ^ RESEND ^ NUM_RETRY ^ TERMINATOR;
    //cs = DST_NODE ^ BYTE ^ seq ^ RESPONSE_MSG_DISABLED ^ TERMINATOR;
    for (int i = 0; i < len; i++) { // Payload
        send_one(pbuf[i]);
        cs = cs ^ pbuf[i];
    }
    //--- Checksum
    send_one(cs); // Checksum
    //printf("cs: %x\n", cs);
}

/**
 * Note: this API is just for receiving an one-byte payload.
 */
void twelite_uart_rx(uint8_t* buf, uint8_t *cmd, uint8_t *seq) {
    if (buf[EOT_POS] != EOT) {
      HAL_Delay(RESET_DELAY);
      NVIC_SystemReset();
      //return false;
    } else {
      *seq = buf[SEQ_NUMBER_POS];
      *cmd = buf[PAYLOAD_POS];
    }
}
