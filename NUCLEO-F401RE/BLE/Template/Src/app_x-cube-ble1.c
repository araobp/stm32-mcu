/**
  ******************************************************************************
  * File Name          : app_x-cube-ble1.c
  * Description        : Implementation file
  *             
  ******************************************************************************
  *
  * COPYRIGHT 2019 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_X_CUBE_BLE1_C
#define __APP_X_CUBE_BLE1_C
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-ble1.h"

/* USER CODE BEGING TEMPLATE */
#include <inference_service.h>

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

 /* USER CODE END TEMPLATE */


#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void){
  uint32_t ms = HAL_GetTick();
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}
#endif

void MX_BlueNRG_MS_Init(void)
{
  /* USER CODE BEGIN SV */
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


  /* USER CODE END SV */
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  /* USER CODE BEGIN BlueNRG_MS_Process_PreTreatment */
  User_Process();
  hci_user_evt_proc();

  /* USER CODE END BlueNRG_MS_Process_PreTreatment */
  

  /* USER CODE BEGIN BlueNRG_MS_Process_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PostTreatment */
}

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
