/* Includes ------------------------------------------------------------------*/
#include <inference_service.h>
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"

#include "app_x-cube-ble1.h"

/* Private variables ---------------------------------------------------------*/
volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
volatile uint8_t start_read_tx_char_handle = FALSE;
volatile uint8_t start_read_rx_char_handle = FALSE;
volatile uint8_t end_read_tx_char_handle = FALSE;
volatile uint8_t end_read_rx_char_handle = FALSE;

uint16_t tx_handle;
uint16_t rx_handle;

uint16_t servHandle, TXCharHandle, RXCharHandle;

/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
  do {\
  	uuid_struct.uuid128[0] = uuid_0; uuid_struct.uuid128[1] = uuid_1; uuid_struct.uuid128[2] = uuid_2; uuid_struct.uuid128[3] = uuid_3; \
	uuid_struct.uuid128[4] = uuid_4; uuid_struct.uuid128[5] = uuid_5; uuid_struct.uuid128[6] = uuid_6; uuid_struct.uuid128[7] = uuid_7; \
	uuid_struct.uuid128[8] = uuid_8; uuid_struct.uuid128[9] = uuid_9; uuid_struct.uuid128[10] = uuid_10; uuid_struct.uuid128[11] = uuid_11; \
	uuid_struct.uuid128[12] = uuid_12; uuid_struct.uuid128[13] = uuid_13; uuid_struct.uuid128[14] = uuid_14; uuid_struct.uuid128[15] = uuid_15; \
	}while(0)

tBleStatus Add_Service(void) {
  tBleStatus ret;

  /*
   * Service: 11223344-5566-7788-9900-aabbccddeeff
   * Char TX: 01020304-0506-0708-0900-0a0b0c0d0e0f
   * Char Rx: 01020304-0506-0708-0900-aabbccddeeff
   */
  const uint8_t service_uuid[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
  const uint8_t charUuidTX[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  const uint8_t charUuidRX[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

  ret = aci_gatt_add_serv(UUID_TYPE_128, service_uuid, PRIMARY_SERVICE, 7,
      &servHandle); /* original is 9?? */
  if (ret != BLE_STATUS_SUCCESS)
    goto fail;

  ret = aci_gatt_add_char(servHandle, UUID_TYPE_128, charUuidTX, 20,
  CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, 0, 16, 1, &TXCharHandle);
  if (ret != BLE_STATUS_SUCCESS)
    goto fail;

  ret = aci_gatt_add_char(servHandle, UUID_TYPE_128, charUuidRX, 20,
  CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE,
  GATT_NOTIFY_ATTRIBUTE_WRITE, 16, 1, &RXCharHandle);
  if (ret != BLE_STATUS_SUCCESS)
    goto fail;

  PRINTF("Service added.\nTX Char Handle %04X, RX Char Handle %04X\n", TXCharHandle, RXCharHandle);
  return BLE_STATUS_SUCCESS;

  fail: PRINTF("Error while adding Service.\n");
  return BLE_STATUS_ERROR;
}

/**
 * @brief  Make the device connectable
 * @param  None 
 * @retval None
 */
void Make_Connection(void) {

  tBleStatus ret;

  const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'A', 'S', 'C' };

  /* disable scan response */
  hci_le_set_scan_resp_data(0, NULL);

  PRINTF("General Discoverable Mode ");
  /*
   Advertising_Event_Type, Adv_Interval_Min, Adv_Interval_Max, Address_Type, Adv_Filter_Policy,
   Local_Name_Length, Local_Name, Service_Uuid_Length, Service_Uuid_List, Slave_Conn_Interval_Min,
   Slave_Conn_Interval_Max
   */
  ret = aci_gap_set_discoverable(ADV_DATA_TYPE, ADV_INTERV_MIN, ADV_INTERV_MAX,
  PUBLIC_ADDR,
  NO_WHITE_LIST_USE, 4, local_name, 0, NULL, 0, 0);
  PRINTF("%d\n",ret);
}

/**
 * @brief  Discovery TX characteristic handle by UUID 128 bits
 * @param  None 
 * @retval None
 */
void startReadTXCharHandle(void) {
  if (!start_read_tx_char_handle) {
    PRINTF("Start reading TX Char Handle\n");

    const uint8_t charUuid128_TX[16] = { 0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08,
        0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe1, 0xf2, 0x73, 0xd9 };
    aci_gatt_disc_charac_by_uuid(connection_handle, 0x0001, 0xFFFF,
    UUID_TYPE_128, charUuid128_TX);
    start_read_tx_char_handle = TRUE;
  }
}

/**
 * @brief  Discovery RX characteristic handle by UUID 128 bits
 * @param  None 
 * @retval None
 */
void startReadRXCharHandle(void) {
  if (!start_read_rx_char_handle) {
    PRINTF("Start reading RX Char Handle\n");

    const uint8_t charUuid128_RX[16] = { 0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08,
        0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xe2, 0xf2, 0x73, 0xd9 };
    aci_gatt_disc_charac_by_uuid(connection_handle, 0x0001, 0xFFFF,
    UUID_TYPE_128, charUuid128_RX);
    start_read_rx_char_handle = TRUE;
  }
}

void receiveData(uint8_t* data_buffer, uint8_t Nb_bytes) {
  for (int i = 0; i < Nb_bytes; i++) {
    printf("%c", data_buffer[i]);
  }
  fflush(stdout);
}

void sendData(uint8_t* data_buffer, uint8_t Nb_bytes) {
  aci_gatt_update_char_value(servHandle, TXCharHandle, 0, Nb_bytes,
      data_buffer);
}

void enableNotification(void) {
  uint8_t client_char_conf_data[] = { 0x01, 0x00 }; // Enable notifications

  uint32_t tickstart = HAL_GetTick();

  while (aci_gatt_write_charac_descriptor(connection_handle, tx_handle + 2, 2,
      client_char_conf_data) == BLE_STATUS_NOT_ALLOWED) {
    /* Radio is busy */
    if ((HAL_GetTick() - tickstart) > (10 * HCI_DEFAULT_TIMEOUT_MS))
      break;
  }
  notification_enabled = TRUE;
}

void Attribute_Modified_CB(uint16_t handle, uint8_t data_length,
    uint8_t *att_data) {
  if (handle == RXCharHandle + 1) {
    receiveData(att_data, data_length);
  } else if (handle == TXCharHandle + 2) {
    if (att_data[0] == 0x01)
      notification_enabled = TRUE;
  }
}

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle) {
  connected = TRUE;
  connection_handle = handle;

  printf("Connected to device:");
  for (int i = 5; i > 0; i--) {
    printf("%02X-", addr[i]);
  }
  printf("%02X\n", addr[0]);
}

void GAP_DisconnectionComplete_CB(void) {
  connected = FALSE;

  printf("Disconnected\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
  start_read_tx_char_handle = FALSE;
  start_read_rx_char_handle = FALSE;
  end_read_tx_char_handle = FALSE;
  end_read_rx_char_handle = FALSE;
}

void GATT_Notification_CB(uint16_t attr_handle, uint8_t attr_len,
    uint8_t *attr_value) {
  if (attr_handle == tx_handle + 1) {
    receiveData(attr_value, attr_len);
  }
}

void user_notify(void * pData) {
  hci_uart_pckt *hci_pckt = pData;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*) hci_pckt->data;

  if (hci_pckt->type != HCI_EVENT_PKT)
    return;

  switch (event_pckt->evt) {

  case EVT_DISCONN_COMPLETE: {
    GAP_DisconnectionComplete_CB();
  }
    break;

  case EVT_LE_META_EVENT: {
    evt_le_meta_event *evt = (void *) event_pckt->data;

    switch (evt->subevent) {
    case EVT_LE_CONN_COMPLETE: {
      evt_le_connection_complete *cc = (void *) evt->data;
      GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
    }
      break;
    }
  }
    break;

  case EVT_VENDOR: {
    evt_blue_aci *blue_evt = (void*) event_pckt->data;
    switch (blue_evt->ecode) {

    case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED: {
      evt_gatt_attr_modified_IDB05A1 *evt =
          (evt_gatt_attr_modified_IDB05A1*) blue_evt->data;
      Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data);
    }
      break;
    case EVT_BLUE_GATT_NOTIFICATION: {
      evt_gatt_attr_notification *evt =
          (evt_gatt_attr_notification*) blue_evt->data;
      GATT_Notification_CB(evt->attr_handle, evt->event_data_length - 2,
          evt->attr_value);
    }
      break;
    default:
      break;
    }
  }
    break;
  }
}

void User_Process(void) {
  uint8_t data[] = { 0 };
  static uint8_t inference_result = 0;
  static uint32_t prev_time = 0;
  uint32_t current_time;
  if (set_connectable) {
    /* Establish connection with remote device */
    Make_Connection();
    set_connectable = FALSE;
  }

  if (connected && notification_enabled) {
    current_time = HAL_GetTick();
    if ( (current_time - prev_time) >= 1000) {  // every 1 sec
      data[0] = inference_result++;
      printf("%d\n", data[0]);
      sendData(data, sizeof(data));
      prev_time = current_time;
    }
  }

}
