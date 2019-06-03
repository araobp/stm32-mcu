/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "app_x-cube-nfc4.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include "nfc04a1.h"
#include "nfc04a1_nfctag.h"
#include "app_nfc_uri.h"
//#include "rn4020.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
  STANDBY, PHASE1, PHASE2
} system_status;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/**
 * T1 is to simulate time taken for extra processing to add additional data
 * to the URI (optional).
 * T2 is expiration time of the URI.
 */
#define T1 100
#define T2 ( 3000 + T1 )
#define T3 2000

#define BUFSIZE 256


#define CMD(cmd) (strcmp((char *)uart_rx_buf, cmd) == 0)

//#define SLEEP_ENABLED

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// RF field status
volatile bool field_changed = false;

// Command reception from uart2
volatile bool command_received = false;
uint8_t uart_rx_buf[BUFSIZE];
uint8_t uart_rx_data;

// Characteristics data simulation
char characteristics[] = "woman,young";  // Some characteristics of the user

// Enable/disable serial number increment
volatile bool increment = true;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void enter_sleep_mode(void) {
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  HAL_ResumeTick();
}

/**
 * This function is called when RF field change has just been
 * detected.
 */
char* extraProcess() {
  //  Extra processing to add additional data to the URI.
  /*
   * <<< Some processing here... >>>
   */
  return characteristics;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //uint8_t ITStatus;
  uint32_t tick_prev = 0;
  uint32_t tick_current = 0;
  uint32_t elapsed_time;
  system_status phase = STANDBY;

  char *pData;

  char base_url[BUFSIZE];

  // Notify BLE central of RF field change
  //uint8_t RF_FIELD_CHANGED[] = "RF_FIELD_CHANGED";
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_NFC_Init();
  /* USER CODE BEGIN 2 */
  NFC04A1_LED_Off( YELLOW_LED );

  NFC04A1_GPO_Init();

  printf("///// START /////\n");

  init_NFC_tag(false);
  lock_nfc();

  // Read the base URL from Area 2 on EEPROM
  read_data_area2((uint8_t *)base_url);

  // Command reception
  HAL_UART_Receive_IT(&huart2, &uart_rx_data, 1);

  // NFC04A1_NFCTAG_WriteRFMngt_Dyn(0, 0b00000001 );  // RF_DISABLED
  NFC04A1_NFCTAG_SetRFDisable_Dyn(0);

#ifdef SLEEP_ENABLED
  enter_sleep_mode();
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifndef SLEEP_ENABLED
    if (field_changed) {
#endif
      if (phase == STANDBY) {
        NFC04A1_LED_On( GREEN_LED );
        printf("PHASE 1: RF field change detected\n");
        //notify(RF_FIELD_CHANGED, sizeof(RF_FIELD_CHANGED)-1);  // Notify the event to BLE entral
        pData = extraProcess();
        generate_URI_with_serial_number('4', (char *)base_url, (char *)pData, increment);
        tick_prev = HAL_GetTick();
        phase = PHASE1;
      }
#ifndef SLEEP_ENABLED
      field_changed = false;
    }
#endif
    if (phase != STANDBY) {
       tick_current = HAL_GetTick();
       if (tick_prev > tick_current) {
          elapsed_time = tick_current - tick_prev;
       } else {
         elapsed_time = tick_current + (0xffffffff - tick_prev);
       }
       if (phase == PHASE1 && elapsed_time > T1) {
         // NFC04A1_NFCTAG_WriteRFMngt_Dyn(0, 0b00000000 );  // RF woken up
         NFC04A1_NFCTAG_ResetRFDisable_Dyn(0);

         NFC04A1_LED_Off( GREEN_LED );
         NFC04A1_LED_On( BLUE_LED );
         printf("PHASE 2: RF enabled to transfer URI\n");
         phase = PHASE2;
       /*
       } else if (phase == PHASE2 && elapsed_time <= T2) {
         printf("elapsed time: %ld\n", elapsed_time);
       */
       } else if (phase == PHASE2 && elapsed_time > T2) {
         printf("elapsed time: %ld\n", elapsed_time);
         //NFC04A1_NFCTAG_WriteRFMngt_Dyn(0, 0b00000001 );  // RF_DISABLED
         NFC04A1_NFCTAG_SetRFDisable_Dyn(0);
         NFC04A1_LED_Off( BLUE_LED );
         NFC04A1_LED_On( YELLOW_LED );
         printf("STANDBY: RF disabled\n\n");
         phase = STANDBY;
         HAL_Delay(T3);
         NFC04A1_LED_Off( YELLOW_LED );
#ifdef SLEEP_ENABLED
         enter_sleep_mode();
       }
#endif
       }
#ifndef SLEEP_ENABLED
    }
#endif

    // Command parser
    if (command_received) {
      if (strcmp((char *)uart_rx_buf, ".r") == 0) {  // Reset NFC tag
        printf("RESET...\n\n");
        init_NFC_tag(true);
      } else if (strcmp((char *)uart_rx_buf, ".l") == 0) {
        printf("LOCK NFC...\n\n");
        lock_nfc();
      } else if (strcmp((char *)uart_rx_buf, ".u") == 0) {
        printf("UNLOCK NFC...\n\n");
        unlock_nfc();
      } else if (CMD(".d")) {
    	increment = false;
      } else if (CMD(".e")) {
    	increment = true;
      } else {
        // Write URL
        write_data_area2(uart_rx_buf, strlen((char *)uart_rx_buf)+1);
        strcpy(base_url, (char *)uart_rx_buf);
      }
      command_received = false;
    }
    /* USER CODE END WHILE */

  MX_NFC_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void BSP_GPO_Callback() {
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
#ifndef SLEEP_ENABLED
  if (!field_changed) {
    field_changed = true;
  }
#endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {

  static int idx = 0;

  if (!command_received) {
    if (uart_rx_data == '\n') {
      uart_rx_buf[idx] = '\0';
      idx = 0;
      command_received = true;
    } else {
      uart_rx_buf[idx++] = uart_rx_data;
    }

    HAL_UART_Receive_IT(&huart2, &uart_rx_data, 1);
  }
}

int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *) ptr, (uint16_t) len, 0xFFFFFFFF);
  return len;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
