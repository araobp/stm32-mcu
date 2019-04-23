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

#include "hci_tl.h"
#include "sample_service.h"
#include "role_type.h"
#include "bluenrg_utils.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private defines -----------------------------------------------------------*/
/**
 * Define the role here only if it is not already defined in the project options
 * For the CLIENT_ROLE comment the line below 
 * For the SERVER_ROLE uncomment the line below
 */
#define SERVER_ROLE

#define BDADDR_SIZE 6
 
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t bnrg_expansion_board = IDB04A1; /* at startup, suppose the X-NUCLEO-IDB04A1 is used */  
static volatile uint8_t user_button_init_state = 1;
static volatile uint8_t user_button_pressed = 0;

#ifdef SERVER_ROLE
  BLE_RoleTypeDef BLE_Role = SERVER;
#else
  BLE_RoleTypeDef BLE_Role = CLIENT;
#endif

extern volatile uint8_t set_connectable;
extern volatile int     connected;
extern volatile uint8_t notification_enabled;

extern volatile uint8_t end_read_tx_char_handle;
extern volatile uint8_t end_read_rx_char_handle;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void User_Process(void);
static void User_Init(void);

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

void MX_BlueNRG_MS_Init(void)
{
  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */
  uint8_t CLIENT_BDADDR[] = {0xbb, 0x00, 0x00, 0xE1, 0x80, 0x02};
  uint8_t SERVER_BDADDR[] = {0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02};
  uint8_t bdaddr[BDADDR_SIZE];
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  
  uint8_t  hwVersion;
  uint16_t fwVersion;
  int ret;
  
  User_Init();
  
  /* Get the User Button initial state */
  user_button_init_state = BSP_PB_GetState(BUTTON_KEY);
  
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
  
  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1; 
  }
  
  if (BLE_Role == CLIENT) {
    BLUENRG_memcpy(bdaddr, CLIENT_BDADDR, sizeof(CLIENT_BDADDR));
  } else {
    BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));
  }
  
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);
  if (ret) {
    printf("Setting BD_ADDR failed 0x%02x.\n", ret);
  }
  
  ret = aci_gatt_init();    
  if (ret) {
    printf("GATT_Init failed.\n");
  }
  
  if (BLE_Role == SERVER) {
    if (bnrg_expansion_board == IDB05A1) {
      ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
    }
    else {
      ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
    }
  }
  else {
    if (bnrg_expansion_board == IDB05A1) {
      ret = aci_gap_init_IDB05A1(GAP_CENTRAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
    }
    else {
      ret = aci_gap_init_IDB04A1(GAP_CENTRAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
    }
  }
  
  if (ret != BLE_STATUS_SUCCESS) {
    printf("GAP_Init failed.\n");
  }
    
  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    printf("BLE Stack Initialized.\n");
  }
  
  if (BLE_Role == SERVER) {
    printf("SERVER: BLE Stack Initialized\n");
    ret = Add_Sample_Service();
    
    if (ret == BLE_STATUS_SUCCESS)
      printf("Service added successfully.\n");
    else
      printf("Error while adding service.\n");
    
  } else {
    printf("CLIENT: BLE Stack Initialized\n");
  }
  
  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);

  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  /* USER CODE BEGIN BlueNRG_MS_Process_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PreTreatment */
  
  User_Process();  
  hci_user_evt_proc();

  /* USER CODE BEGIN BlueNRG_MS_Process_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PostTreatment */
}

/**
 * @brief  Initialize User process.
 *
 * @param  None
 * @retval None
 */
static void User_Init(void)
{
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED2);
    
  BSP_COM_Init(COM1); 
}

/**
 * @brief  Configure the device as Client or Server and manage the communication
 *         between a client and a server.
 *
 * @param  None
 * @retval None
 */
static void User_Process(void)
{
  if (set_connectable) 
  {
    /* Establish connection with remote device */
    Make_Connection();
    set_connectable = FALSE;
    user_button_init_state = BSP_PB_GetState(BUTTON_KEY);
  }
  
  if (BLE_Role == CLIENT) 
  {
    /* Start TX handle Characteristic dynamic discovery if not yet done */
    if (connected && !end_read_tx_char_handle){
      startReadTXCharHandle();
    }
    /* Start RX handle Characteristic dynamic discovery if not yet done */
    else if (connected && !end_read_rx_char_handle){      
      startReadRXCharHandle();
    }
    
    if (connected && end_read_tx_char_handle && end_read_rx_char_handle && !notification_enabled) 
    {
      BSP_LED_Off(LED2); //end of the connection and chars discovery phase
      enableNotification();
    }
  }  

  /* Check if the User Button has been pushed */
  if (user_button_pressed) 
  {
    /* Debouncing */
    HAL_Delay(50);
    
    /* Wait until the User Button is released */
    while (BSP_PB_GetState(BUTTON_KEY) == !user_button_init_state);
    
    /* Debouncing */
    HAL_Delay(50);
    
    if (connected && notification_enabled)
    {
      /* Send a toggle command to the remote device */
      uint8_t data[20] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J'};
      sendData(data, sizeof(data));
      
      //BSP_LED_Toggle(LED2);  /* Toggle the LED2 locally. */
                               /* If uncommented be sure the BSP_LED_Init(LED2)
                                * is called in main().
                                * E.g. it can be enabled for debugging. */
    }
    
    /* Reset the User Button flag */
    user_button_pressed = 0;
  } 
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Set the User Button flag */
  user_button_pressed = 1;
}

#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
