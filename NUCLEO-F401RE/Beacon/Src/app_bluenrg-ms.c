/**
  ******************************************************************************
  * File Name          : app_bluenrg-ms.c
  * Description        : Implementation file
  *             
  ******************************************************************************
  *
  * COPYRIGHT 2018 STMicroelectronics
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
#ifndef __APP_BLUENRGMS_C
#define __APP_BLUENRGMS_C
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_bluenrg-ms.h"

#include <string.h>
#include "hci_le.h"
#include "hci_tl.h"
#include "sensor_service.h"
#include "eddystone_beacon.h"
#include "bluenrg_utils.h"
#include "stm32f4_nucleo_f401re.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/**
 * Define the beacon type here only if it is not already defined in the project 
 * options.
 * For the URL beacon type uncomment the line below.
 * Otherwise, if it is not already defined in the project options, the UID 
 * beacon type is set.
 */
//#define EDDYSTONE_BEACON_TYPE EDDYSTONE_URL_BEACON_TYPE

#ifndef EDDYSTONE_BEACON_TYPE
  #define EDDYSTONE_BEACON_TYPE EDDYSTONE_UID_BEACON_TYPE
#endif

#define IDB04A1 0
#define IDB05A1 1

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void EnterStopMode(void);
static void User_Init(void);
static void User_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

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

void MX_X_CUBE_BLE1_Init(void)
{
  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */
  
  /* USER CODE BEGIN X_CUBE_BLE1_Init_PreTreatment */
  
  /* USER CODE END X_CUBE_BLE1_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */
  tBleStatus ret = BLE_STATUS_SUCCESS;

  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  uint8_t SERVER_BDADDR[] = { MAC_ADDRESS };

  uint8_t  hwVersion;
  uint16_t fwVersion;
  uint8_t bnrg_expansion_board = IDB04A1; /* at startup, suppose the X-NUCLEO-IDB04A1 is used */

  User_Init();
    
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

  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1; 
  }
  
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  SERVER_BDADDR);

  if (ret != BLE_STATUS_SUCCESS)
  {
    __asm("nop");
  }

  ret = aci_gatt_init();

  if (ret != BLE_STATUS_SUCCESS)
  {
    __asm("nop");
  }

  if (bnrg_expansion_board == IDB05A1) {
    ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  else {
    ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  
  if (ret != BLE_STATUS_SUCCESS)
  {
    __asm("nop");
  }

  ret = aci_hal_set_tx_power_level(1,4);

  if (ret != BLE_STATUS_SUCCESS)
  {
    __asm("nop");
  }

  /* Initialize beacon services */
  if (EDDYSTONE_BEACON_TYPE & EDDYSTONE_UID_BEACON_TYPE)
  {
    EddystoneUID_Start();
  }
  if (EDDYSTONE_BEACON_TYPE & EDDYSTONE_URL_BEACON_TYPE)
  {
    EddystoneURL_Start();
  }

  /* USER CODE BEGIN X_CUBE_BLE1_Init_PostTreatment */
  
  /* USER CODE END X_CUBE_BLE1_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_X_CUBE_BLE1_Process(void)
{
  /* USER CODE BEGIN X_CUBE_BLE1_Process_PreTreatment */
  
  /* USER CODE END X_CUBE_BLE1_Process_PreTreatment */
  
  User_Process();  
  hci_user_evt_proc();

  /* USER CODE BEGIN X_CUBE_BLE1_Process_PostTreatment */
  
  /* USER CODE END X_CUBE_BLE1_Process_PostTreatment */
}

/**
 * @brief  Initialize User process.
 *
 * @param  None
 * @retval None
 */
static void User_Init(void)
{
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  BSP_LED_Init(LED2);
  
  BSP_COM_Init(COM1); 
}

/**
 * @brief  User application process
 * @param  None
 * @retval None
 */
static void User_Process(void)
{
  EnterStopMode(); 
}

/**
 * @brief  Enter the STOP mode
 * @param  None
 * @retval None
 */
void EnterStopMode(void)
{
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

#ifdef __cplusplus
}
#endif
#endif /* __APP_BLUENRGMS_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
