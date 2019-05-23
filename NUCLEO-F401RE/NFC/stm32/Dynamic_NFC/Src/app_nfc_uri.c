/*
 * app_nfc_uri.c
 *
 *  Created on: 2019/04/26
 */

#include "app_nfc_uri.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int32_t status;

// ST25 unique device ID
ST25DV_UID uid;

// GPO management
uint8_t GPOConfig;
ST25DV_EN_STATUS GPO_en;

// Password for I2C security (default: all zero)
ST25DV_PASSWD passwd;

// Serial Number
uint32_t sno = 0;

void write_data_area3(uint32_t sno);
uint32_t read_data_area3(void);

/**
 * Initialize NFC tag with the predefined parameters
 * for this device.
 */
void init_NFC_tag(bool reset) {
  passwd.MsbPasswd = 0;
  passwd.LsbPasswd = 0;

  printf("--- GPO ---\n");
  NFC04A1_NFCTAG_ReadUID(0, &uid);
  printf("UID: 0x%08lx", uid.MsbUid);
  printf("%08lx\n", uid.LsbUid);

  printf("GPO: %ld\n", NFC04A1_GPO_ReadPin());

  NFC04A1_NFCTAG_ReadGPO_Dyn(0, &GPOConfig );
  printf("GPO Config: %0x\n", GPOConfig);

  NFC04A1_NFCTAG_GetGPO_en_Dyn(0, &GPO_en );
  printf("GPO Config En: %0x\n\n", GPO_en);

  printf("--- I2C security ---\n");
  status = NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  printf("Status of PresentI2CPassword: %ld\n\n", status);

  if (reset) {
    printf("--- EEPROM zones ---\n");
    status = NFC04A1_NFCTAG_CreateUserZone(0, 32*8, 31*8+4, 4, 0);
    printf("Status of CreateUserZone: %ld\n\n", status);

    printf("--- SNO reset ---\n");
    write_data_area3(0);
    sno = 0;
    printf("Serial number reset to 0\n");
  }

}

/**
 * Write an URI to Area 1
 */
void URI_write(char identifier, char *pUri) {

  sURI_Info URI;

  switch(identifier) {
    case '3':
      strcpy( URI.protocol, URI_ID_0x03_STRING);
      break;
    case '4':
      strcpy( URI.protocol, URI_ID_0x04_STRING);
      break;
    default:
      break;
  }

  strcpy(URI.URI_Message, pUri);
  strcpy(URI.Information, "\0" );

  /* Write NDEF to EEPROM */
  HAL_Delay(5);
  while( NDEF_WriteURI( &URI ) != NDEF_OK );
}

/**
 * Read an URI from Area 1
 */
void URI_read(sURI_Info *pUri) {
  sRecordInfo_t record;
  NDEF_ReadNDEF(NDEF_Buffer);
  NDEF_IdentifyNDEF(&record, NDEF_Buffer);
  NDEF_ReadURI(&record, pUri);
}

/**
 * Write a base URL to Area 2 (256 - 4 bytes length)
 */
void write_data_area2(uint8_t *pData, uint8_t len) {
  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  NFC04A1_NFCTAG_WriteData(0, &len, 256, 1);
  HAL_Delay(50);
  NFC04A1_NFCTAG_WriteData(0, pData, 256+1, len);
}

/**
 * Read a base URL from Area 2 (256 - 4 bytes length)
 */
uint8_t read_data_area2(uint8_t *pData) {
  uint8_t len;
  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  NFC04A1_NFCTAG_ReadData(0, &len, 256, 1);
  HAL_Delay(50);
  NFC04A1_NFCTAG_ReadData(0, pData, 256+1, len);

  printf("--- Area2 data ---\n");
  printf("Data: %s\n\n", pData);

  return len;
}

/**
 * Write a serial number to Area 3 (uint32_t: 4bytes length)
 */
void write_data_area3(uint32_t sno) {
  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  NFC04A1_NFCTAG_WriteData(0, (uint8_t *)&sno, 508, 4);
}

/**
 * Read a serial number from Area 3 (uint32_t: 4bytes length)
 */
uint32_t read_data_area3(void) {
  uint32_t data;
  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  NFC04A1_NFCTAG_ReadData(0, (uint8_t *)&data, 508, 4);
  return data;
}

/**
 * Write URL with a serial number to NFC tag
 */
void generate_URI_with_serial_number(char identifier, char *pUri, char *pData) {

  char buf[256] = { 0 };
  int len;

  if (sno == 0) {
    sno = read_data_area3();
  }

  strcpy(buf, pUri);
  len = strlen(pUri);

  if (pData != NULL) {
    buf[len++] = '&';
    buf[len++] = 'd';
    buf[len++] = 'a';
    buf[len++] = 't';
    buf[len++] = '=';
    strcpy(buf+len, pData);
    len = len + strlen(pData);
  }

  buf[len++] = '&';
  buf[len++] = 's';
  buf[len++] = 'n';
  buf[len++] = 'o';
  buf[len++] = '=';
  itoa(sno, buf+len, 10);

  printf("sno: %ld\n", sno);
  printf("uri w/ sno: %s\n", buf);

  write_data_area3(++sno);
  URI_write(identifier, buf);
}

/**
 * Lock NFC tag from RF access
 */
void lock_nfc(void) {
  ST25DV_RF_PROT_ZONE RfProtZone;

  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  RfProtZone.PasswdCtrl = ST25DV_NOT_PROTECTED;
  RfProtZone.RWprotection = ST25DV_WRITE_PROT;
  NFC04A1_NFCTAG_WriteRFZxSS(0, ST25DV_PROT_ZONE1, RfProtZone);

  HAL_Delay(50);
  RfProtZone.PasswdCtrl = ST25DV_NOT_PROTECTED;
  RfProtZone.RWprotection = ST25DV_READWRITE_PROT;
  NFC04A1_NFCTAG_WriteRFZxSS(0, ST25DV_PROT_ZONE1, RfProtZone);

  HAL_Delay(50);
  NFC04A1_NFCTAG_WriteRFZxSS(0, ST25DV_PROT_ZONE3, RfProtZone);
}

/**
 * Unlock NFC tag from RF access for Area 1 only
 */
void unlock_nfc(void) {
  ST25DV_RF_PROT_ZONE RfProtZone;

  NFC04A1_NFCTAG_PresentI2CPassword(0, passwd);
  HAL_Delay(50);
  RfProtZone.PasswdCtrl = ST25DV_NOT_PROTECTED;
  RfProtZone.RWprotection = ST25DV_NO_PROT;
  NFC04A1_NFCTAG_WriteRFZxSS(0, ST25DV_PROT_ZONE1, RfProtZone);
}
