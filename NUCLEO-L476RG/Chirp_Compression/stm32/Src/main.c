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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "dfsdm.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "stdio.h"
#include "stdbool.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "chirp.h"
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
/* Private variables ---------------------------------------------------------*/

// flag: "new PCM data has just been copied to buf"
volatile buffer_cplt cplt = A;
volatile bool new_pcm_data;

// UART input/output
volatile bool output_result = false;
volatile bool wave_out = false;
uint8_t rxbuf[1];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// overlap addition
void overlap_add(float32_t *in, float32_t *out) {

  static float32_t buf[NN] = { 0.0f };
  const uint len = (FRAMES_IN_BUF - 1) * NN;

  // out[A|B|C|D] -> out[B|C|D|-]
  arm_copy_f32(out+NN, out, len);
  // [A|-] + buf[prev B] -> out[-|-|-|A]
  arm_add_f32(in, buf, out+len, NN);
  // [-|B] --> [B]
  arm_copy_f32(in+NN, buf, NN);
}

#define NUM_MEANS 512

// AC coupling (to remove DC)
void apply_ac_coupling(float32_t *signal) {
  float32_t mean;
  static float32_t mean_hist[NUM_MEANS] = { 0.0f };
  arm_copy_f32(mean_hist+1, mean_hist, NUM_MEANS-1);
  arm_mean_f32(signal, NN, mean_hist+NUM_MEANS-1);
  arm_mean_f32(signal, NUM_MEANS, &mean);
  arm_offset_f32(signal, -mean, signal, NN);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  // Buffer for FFT/IFFT processing
  float32_t signal[NN*2] = { 0.0f };

  // Compressed wave (chirp compression)
  float32_t compressed[NN*4] = { 0.0f };

  // DMA peripheral to memory buffer
  int32_t dfsdm_buf[NN*2] = { 0 };

  // UART buffer
  uint8_t uart_buf[NN*FRAMES_IN_BUF*2];

  // Audio sample rate and period
  float fs;
  float sampling_period;

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_DFSDM1_Init();
  /* USER CODE BEGIN 2 */

  // FFT sampling rate
  fs = SystemCoreClock / hdfsdm1_channel2.Init.OutputClock.Divider
      / hdfsdm1_filter0.Init.FilterParam.Oversampling
      / hdfsdm1_filter0.Init.FilterParam.IntOversampling;

  // Initialize reference chirp signal
  init_ref_chirp(fs);

  // Enable UART receive interrupt
  HAL_UART_Receive_IT(&huart2, rxbuf, 1);

  // Enable DMA from DFSDM to buf (peripheral to memory)
  if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, dfsdm_buf, NN*2)
      != HAL_OK) {
    Error_Handler();
  }

  // Show starting message

  printf("/// Chirp compression experiment ///\n\n");
  printf("Sampling rate: %4.1f(kHz)\n", (float) fs / 1000.0f);
  sampling_period = 1.0f / fs * NN;
  printf("Sampling period: %.1f(msec), Samples per period: %ld\n\n",
      sampling_period * 1000.0f, NN);
  printf("Push USER button to output single-shot wave form\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

    // Wait for next PCM samples from M1
    if (new_pcm_data) {

      if (cplt == A) {   // [NN|--]
        for (int n = 0; n < NN; n++) {
          signal[n] = (float32_t) (dfsdm_buf[n] >> 9);
        }
      } else if (cplt == B) {   // [--|NN]
        for (int n = 0; n < NN; n++) {
          signal[n] = (float32_t) (dfsdm_buf[n+NN] >> 9);
        }
      }

      apply_ac_coupling(signal);

      // zero padding: [NN:all 0]
      arm_fill_f32(0.0f, signal + NN, NN);

      // Perform chirp compression
      if (!wave_out) {
        compress_chirp(signal);
      }

      // Overlap add
      overlap_add(signal, compressed);

      // Output result
      if (output_result) {
        //printf("start_time: %lu, finish_time: %lu\n", start_time, finish_time);
        for (int n = 0; n < NN * FRAMES_IN_BUF; n++) {
          uart_buf[n*2] = (uint8_t) (((int16_t)compressed[n]) >> 8);
          uart_buf[n*2+1] = (uint8_t) (((int16_t)compressed[n] & 0x00ff));
        }
        HAL_UART_Transmit_DMA(&huart2, uart_buf, NN * FRAMES_IN_BUF * 2);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        output_result = false;
      }
      new_pcm_data = false;
    }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
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
  /**Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_DFSDM1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief  Half regular conversion complete callback.
 * @param  hdfsdm_filter : DFSDM filter handle.
 * @retval None
 */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(
    DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
  if (!new_pcm_data && (hdfsdm_filter == &hdfsdm1_filter0)) {
    cplt = A;
    new_pcm_data = true;
  }
}

/**
 * @brief  Regular conversion complete callback.
 * @note   In interrupt mode, user has to read conversion value in this function
 using HAL_DFSDM_FilterGetRegularValue.
 * @param  hdfsdm_filter : DFSDM filter handle.
 * @retval None
 */
void HAL_DFSDM_FilterRegConvCpltCallback(
    DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
  if (!new_pcm_data && (hdfsdm_filter == &hdfsdm1_filter0)) {
    cplt = B;
    new_pcm_data = true;
  }
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *) ptr, (uint16_t) len, 0xFFFFFFFF);
  return len;
}

/**
 * @brief  GPIO External Interrupt callback
 * @param  GPIO_Pin
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_13) {  // User button (blue tactile switch)
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    output_result = true;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  switch(rxbuf[0]) {
  case RAW_WAVE:
    wave_out = true;   // Raw data output
    output_result = true;
    break;
  case COMPRESSION:
    wave_out = false;  // Chirp compression result
    output_result = true;
    break;
  }

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  HAL_UART_Receive_IT(&huart2, rxbuf, 1);
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
  while (1) {
  }
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
   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
