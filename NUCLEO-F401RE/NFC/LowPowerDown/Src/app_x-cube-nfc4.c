/**
  ******************************************************************************
  * File Name          :  stmicroelectronics_x-cube-nfc4_1_3_0.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-NFC4.1.3.0 instances.
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

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-nfc4.h"
#include "nfc04a1_nfctag.h"
#include "stm32f4_nucleo_f401re.h"

/** @defgroup ST25_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
uint8_t nfctagid = 0;
int32_t status;
static volatile uint8_t UserChange = 0;
  
 /* Private functions ---------------------------------------------------------*/
void MX_NFC4_LPD_Init(void);
void MX_NFC4_LPD_Process(void);
void BSP_PB_Callback(Button_TypeDef Button);

void MX_NFC_Init(void)
{
  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC4_Library_Init_PreTreatment */
  
  /* USER CODE END NFC4_Library_Init_PreTreatment */

  /* Initialize the peripherals and the NFC4 components */

  MX_NFC4_LPD_Init();

  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */
  
  /* USER CODE BEGIN NFC4_Library_Init_PostTreatment */
  
  /* USER CODE END NFC4_Library_Init_PostTreatment */
}
/*
 * LM background task
 */
void MX_NFC_Process(void)
{
  /* USER CODE BEGIN NFC4_Library_Process */

  MX_NFC4_LPD_Process();

  /* USER CODE END NFC4_Library_Process */
}

/**
  * @brief  Initialize the LPD feature Example
  * @retval None
  */
void MX_NFC4_LPD_Init(void)
{
  
  /* Configuration of X-NUCLEO-NFC04A1                                          */
  /******************************************************************************/
  /* Init of the Leds on X-NUCLEO-NFC04A1 board */
  
  NFC04A1_LED_Init(GREEN_LED);
  NFC04A1_LED_Init(BLUE_LED);
  NFC04A1_LED_Init(YELLOW_LED);

  NFC04A1_LED_On( GREEN_LED );
  HAL_Delay( 300 );
  NFC04A1_LED_On( BLUE_LED );
  HAL_Delay( 300 );
  NFC04A1_LED_On( YELLOW_LED );
  HAL_Delay( 300 );
  
  /* Init the GPIO Pin of LPD */
  NFC04A1_LPD_Init();
  
 /* Init User button to allow user to change LPD configuration */
  BSP_PB_Init( BUTTON_KEY, BUTTON_MODE_EXTI );
  
 /* Init ST25DV driver */
  while( NFC04A1_NFCTAG_Init(NFC04A1_NFCTAG_INSTANCE) != NFCTAG_OK );
  
  
  NFC04A1_LED_Off( GREEN_LED );
  HAL_Delay( 300 );
  NFC04A1_LED_Off( BLUE_LED );
  HAL_Delay( 300 );
  NFC04A1_LED_Off( YELLOW_LED );
  HAL_Delay( 300 );
  
  if( NFC04A1_LPD_ReadPin( ) == GPIO_PIN_SET )
  {
    /* When LPD is activated, Green led is ON */
    NFC04A1_LED_On( GREEN_LED );
  }
  else
  {
    /* When LPD is deactivated, Green led is OFF */
    NFC04A1_LED_Off( GREEN_LED );
  }
}  
  /**
  * @brief  Process of the EnergyHarvesting application
  * @retval None
  */
void MX_NFC4_LPD_Process(void)
{
  if( UserChange == 1 )
    {
	  UserChange = 0;
	  /*Change the LPD GPIO state*/
	  HAL_GPIO_TogglePin( NFC04A1_LPD_PIN_PORT, NFC04A1_LPD_PIN );
	  NFC04A1_LED_Toggle( GREEN_LED );
      HAL_Delay( 50 );
      status = NFC04A1_NFCTAG_ReadID(NFC04A1_NFCTAG_INSTANCE, &nfctagid );
	  
	  if(status == NFCTAG_OK && (nfctagid != 0))
		{
		nfctagid = 0;
        NFC04A1_LED_On( YELLOW_LED );
		}
      else
		{
        NFC04A1_LED_Off( YELLOW_LED );
		}
	}  	  
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if( Button == BUTTON_KEY )
  {
    UserChange = 1;
  }
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
