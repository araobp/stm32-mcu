/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dct.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  dct2_instance_f32 S;

  dct2_instance_f32 S_2d;

  dct2_instance_f32 S_zigzag_even;

  dct2_instance_f32 S_zigzag_odd;

  float32_t in_data[8] = {
      1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 3.0, 2.0
  };

  float32_t out_data[8] = { 0.0 };

  float32_t in_data_2d[8*8] = {
      1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 3.0, 2.0,
            2.0, 0.0, 0.0, 2.0, 3.0, 4.0, 4.0, 2.0,
            3.0, 0.0, -1.0, 2.0, 3.0, 4.0, 3.0, 2.0,
            4.0, 0.0, -2.0, 2.0, 3.0, 4.0, 2.0, 2.0,
            5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0,
            6.0, 0.0, -4.0, 2.0, 3.0, 4.0, 0.0, 2.0,
            7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0,
            8.0, 0.0, -6.0, 2.0, 3.0, 4.0, -2.0, 2.0
  };

  float32_t out_data_2d[8*8] = { 0.0 };

  float32_t in_data_zigzag_even[8*8] = {
       1.0,  2.0,  6.0,  7.0, 15.0, 16.0, 28.0, 29.0,
       3.0,  5.0,  8.0, 14.0, 17.0, 27.0, 30.0, 43.0,
       4.0,  9.0, 13.0, 18.0, 26.0, 31.0, 42.0, 44.0,
      10.0, 12.0, 19.0, 25.0, 32.0, 41.0, 45.0, 54.0,
      11.0, 20.0, 24.0, 33.0, 40.0, 46.0, 53.0, 55.0,
      21.0, 23.0, 34.0, 39.0, 47.0, 52.0, 56.0, 61.0,
      22.0, 35.0, 38.0, 48.0, 51.0, 57.0, 60.0, 62.0,
      36.0, 37.0, 49.0, 50.0, 58.0, 59.0, 63.0, 64.0
  };

  float32_t out_data_zigzag_even[8*8] = { 0.0 };

  float32_t in_data_zigzag_odd[7*7] = {
       1.0,  2.0,  6.0,  7.0, 15.0, 16.0, 28.0,
       3.0,  5.0,  8.0, 14.0, 17.0, 27.0, 29.0,
       4.0,  9.0, 13.0, 18.0, 26.0, 30.0, 39.0,
      10.0, 12.0, 19.0, 25.0, 31.0, 38.0, 40.0,
      11.0, 20.0, 24.0, 32.0, 37.0, 41.0, 46.0,
      21.0, 23.0, 33.0, 36.0, 42.0, 45.0, 47.0,
      22.0, 34.0, 35.0, 43.0, 44.0, 48.0, 49.0
  };

  float32_t out_data_zigzag_odd[7*7] = { 0.0 };

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
  /* USER CODE BEGIN 2 */

  dct2_init_f32(&S, 8);
  dct2_2d_init_f32(&S_2d, 8, 8);
  dct2_2d_init_f32(&S_zigzag_even, 8, 8);
  dct2_2d_init_f32(&S_zigzag_odd, 7, 7);

  printf ("**** START *********\n");

  printf(">> DCT\n");
  dct2_f32(&S, in_data, out_data, 0);
  for (int i=0; i<8; i++) {
    printf("[%d] %.1f\n", i, out_data[i]);
  }
  printf(">> inverse DCT\n");
  dct2_f32(&S, out_data, in_data, 1);
  for (int i=0; i<8; i++) {
    printf("[%d] %.1f\n", i, in_data[i]);
  }

  printf(">> DCT 2D\n");
  dct2_2d_f32(&S_2d, in_data_2d, out_data_2d, 0);
  for (int i=0; i<8*8; i++) {
    printf("[%d] %.1f\n", i, out_data_2d[i]);
  }
  printf(">> inverse DCT 2D\n");
  dct2_2d_f32(&S_2d, out_data_2d, in_data_2d, 1);
  for (int i=0; i<8*8; i++) {
    printf("[%d] %.1f\n", i, in_data_2d[i]);
  }

  printf(">> Zigzag scan even\n");
  zigzag_scan_f32(&S_zigzag_even, in_data_zigzag_even, out_data_zigzag_even);
  for (int i=0; i<8*8; i++) {
    printf("[%d] %.1f\n", i, out_data_zigzag_even[i]);
  }

  printf(">> Zigzag scan odd\n");
  zigzag_scan_f32(&S_zigzag_odd, in_data_zigzag_odd, out_data_zigzag_odd);
  for (int i=0; i<7*7; i++) {
    printf("[%d] %.1f\n", i, out_data_zigzag_odd[i]);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
