#include <inference_service.h>
#include "app_x-cube-ble1.h"

#include "hci_tl.h"
#include "bluenrg_utils.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"

/* Private defines -----------------------------------------------------------*/
#define BDADDR_SIZE 6

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t set_connectable;
extern volatile int connected;
extern volatile uint8_t notification_enabled;

extern volatile uint8_t end_read_tx_char_handle;
extern volatile uint8_t end_read_rx_char_handle;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

void MX_BlueNRG_MS_Init(void) {
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */

  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */
  uint8_t SERVER_BDADDR[] = { 0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02 };
  uint8_t bdaddr[BDADDR_SIZE];
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

  uint8_t hwVersion;
  uint16_t fwVersion;
  int ret;

  hci_init(user_notify, NULL);

  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);

  /* 
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  hci_reset();

  HAL_Delay(100);

  printf("HWver %d, FWver %d\n", hwVersion, fwVersion);

  BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));

  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
  CONFIG_DATA_PUBADDR_LEN, bdaddr);
  if (ret) {
    printf("Setting BD_ADDR failed 0x%02x.\n", ret);
  }

  ret = aci_gatt_init();
  if (ret) {
    printf("GATT_Init failed.\n");
  }

  ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07,
      &service_handle, &dev_name_char_handle, &appearance_char_handle);

  if (ret != BLE_STATUS_SUCCESS) {
    printf("GAP_Init failed.\n");
  }

  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
  OOB_AUTH_DATA_ABSENT,
  NULL, 7, 16,
  USE_FIXED_PIN_FOR_PAIRING, 123456,
  BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    printf("BLE Stack Initialized.\n");
  }

  printf("SERVER: BLE Stack Initialized\n");
  ret = Add_Service();

  if (ret == BLE_STATUS_SUCCESS)
    printf("Service added successfully.\n");
  else
    printf("Error while adding service.\n");

  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1, 4);

  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */

  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void) {
  /* USER CODE BEGIN BlueNRG_MS_Process_PreTreatment */

  /* USER CODE END BlueNRG_MS_Process_PreTreatment */

  User_Process();
  hci_user_evt_proc();

  /* USER CODE BEGIN BlueNRG_MS_Process_PostTreatment */

  /* USER CODE END BlueNRG_MS_Process_PostTreatment */
}

