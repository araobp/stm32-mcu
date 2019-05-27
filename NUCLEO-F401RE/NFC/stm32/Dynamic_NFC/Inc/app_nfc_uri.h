/*
 * app_nfc_uri.h
 *
 *  Created on: 2019/04/26
 */

#ifndef APP_NFC_URI_H_
#define APP_NFC_URI_H_

#include <stdint.h>
#include "lib_NDEF.h"
#include "lib_NDEF_URI.h"
#include <stdbool.h>

void init_NFC_tag(bool reset);

void URI_write(char identifier, char *pUri);
void URI_read(sURI_Info *pUri);

void write_data_area2(uint8_t *pData, uint8_t len);
uint8_t read_data_area2(uint8_t *pData);
void generate_URI_with_serial_number(char identifier, char *pUri, char *pData, bool increment);

void lock_nfc(void);
void unlock_nfc(void);

#endif /* APP_NFC_URI_H_ */
