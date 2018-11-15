/**
  ******************************************************************************
  * @file  : nucleo_f401re.c
  * @brief : Source file for the BSP Common driver
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/ 
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "stm32f4_nucleo_f401re.h"
#include "stm32f4xx_hal_exti.h"

/** @defgroup BSP BSP
 * @{
 */ 

/** @defgroup NUCLEO_F401RE NUCLEO_F401RE
 * @{
 */   
    
/** @defgroup NUCLEO_F401RE_LOW_LEVEL NUCLEO_F401RE LOW LEVEL
 *  @brief This file provides set of firmware functions to manage Leds and push-button
 *         available on STM32F4xx-Nucleo Kit from STMicroelectronics.
 * @{
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Private_TypesDefinitions NUCLEO_F401RE LOW LEVEL Private TypesDefinitions
 * @{
 */
typedef void (* BSP_EXTI_LineCallback)(void);

/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Private_Defines NUCLEO_F401RE LOW LEVEL Private Defines
 * @{
 */ 

/**
 * @brief STM32F4XX NUCLEO BSP Driver version number V1.2.6
 */  
#define __NUCLEO_F401RE_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __NUCLEO_F401RE_BSP_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __NUCLEO_F401RE_BSP_VERSION_SUB2   (0x06) /*!< [15:8]  sub2 version */
#define __NUCLEO_F401RE_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __NUCLEO_F401RE_BSP_VERSION        ((__NUCLEO_F401RE_BSP_VERSION_MAIN << 24)\
                                                    |(__NUCLEO_F401RE_BSP_VERSION_SUB1 << 16)\
                                                    |(__NUCLEO_F401RE_BSP_VERSION_SUB2 << 8 )\
                                                    |(__NUCLEO_F401RE_BSP_VERSION_RC))
/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_FunctionPrototypes NUCLEO_F401RE LOW LEVEL Private Function Prototypes
 * @{
 */
static void BUTTON_KEY_EXTI_Callback(void);
static void USART2_MspInit(UART_HandleTypeDef *huart);
static void USART2_MspDeInit(UART_HandleTypeDef *huart); 
/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Private_Variables NUCLEO_F401RE LOW LEVEL Private Variables
 * @{
 */
/* Private Variables -----------------------------------------------------------*/
static GPIO_TypeDef*  LED_GPIO_PORT[LEDn] = {LED2_GPIO_PORT};
static const uint16_t LED_GPIO_PIN[LEDn]  = {LED2_GPIO_PIN};
static GPIO_TypeDef*   BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT}; 
static const uint16_t  BUTTON_PIN[BUTTONn]  = {KEY_BUTTON_GPIO_PIN}; 
static const IRQn_Type BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};

EXTI_HandleTypeDef *hExtiButtonHandle[BUTTONn];
USART_TypeDef* COM_USART[COMn] = {COM1_UART};
UART_HandleTypeDef hComHandle[COMn];
#if (USE_COM_LOG == 1)
static COM_TypeDef COM_ActiveLogPort;
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsUsart2MspCbValid = 0;
#endif
__weak HAL_StatusTypeDef MX_USART2_UART_Init(UART_HandleTypeDef* huart);
EXTI_HandleTypeDef hexti13;
const uint32_t BUTTON_EXTI_LINE[BUTTONn] = {KEY_BUTTON_EXTI_LINE};
/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Private_Functions NUCLEO_F401RE LOW LEVEL Private Functions
 * @{
 */ 
/**
 * @brief  This method returns the STM32F4xx NUCLEO BSP Driver revision
 * @retval version: 0xXYZR (8bits for each decimal, R for RC)
 */
int32_t BSP_GetVersion(void)
{
  return __NUCLEO_F401RE_BSP_VERSION;
}

/**
 * @brief  Configures LED on GPIO and/or on MFX.
 * @param  Led: LED to be configured. 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* LED2 is on the same GPIO Port */
  LED2_GPIO_CLK_ENABLE();    
  
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH; 
  gpio_init_structure.Pin   = LED_GPIO_PIN [Led];
  HAL_GPIO_Init(LED_GPIO_PORT [Led], &gpio_init_structure); 
  
  /* By default, turn off LED */
  HAL_GPIO_WritePin(LED_GPIO_PORT [Led], LED_GPIO_PIN [Led], GPIO_PIN_RESET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  DeInit LEDs.
 * @param  Led: LED to be configured. 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @note Led DeInit does not disable the GPIO clock nor disable the Mfx 
 * @retval HAL status
 */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* DeInit the GPIO_LED pin */ 
  gpio_init_structure.Pin = LED_GPIO_PIN [Led];
  
  /* Turn off LED */ 
  HAL_GPIO_WritePin(LED_GPIO_PORT [Led], (uint16_t)LED_GPIO_PIN[Led], GPIO_PIN_RESET);
  HAL_GPIO_DeInit(LED_GPIO_PORT [Led], gpio_init_structure.Pin);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Turns selected LED On.
 * @param  Led: LED to be set on 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT [Led], (uint16_t)LED_GPIO_PIN [Led], GPIO_PIN_SET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Turns selected LED Off. 
 * @param  Led: LED to be set off
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT [Led], (uint16_t)LED_GPIO_PIN [Led], GPIO_PIN_RESET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Toggles the selected LED.
 * @param  Led: LED to be toggled
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_GPIO_PORT[Led], (uint16_t)LED_GPIO_PIN[Led]);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Get the status of the LED.
 * @param  Led: LED for which get the status
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status (1=high, 0=low)
 */
int32_t BSP_LED_GetState(Led_TypeDef Led) 
{ 
  return (int32_t)HAL_GPIO_ReadPin(LED_GPIO_PORT [Led], LED_GPIO_PIN [Led]); 
}
/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *                 This parameter can be one of the following values: 
  *                 @arg  BUTTON_KEY: Key Push Button
  * @param  ButtonMode Button mode
  *                    This parameter can be one of the following values:
  *                    @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *                    @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line 
  *                                            with interrupt generation capability
  * @retval BSP status
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure;
  hExtiButtonHandle[Button] = &hexti13;
  
  static BSP_EXTI_LineCallback ButtonCallback[BUTTONn] ={BUTTON_KEY_EXTI_Callback};                                                
  static uint32_t  BSP_BUTTON_PRIO [BUTTONn] ={BSP_BUTTON_KEY_IT_PRIORITY};  											     
  static const uint32_t BUTTON_EXTI_LINE[BUTTONn] ={KEY_BUTTON_EXTI_LINE};
  
  /* Enable the BUTTON clock*/ 
  KEY_BUTTON_GPIO_CLK_ENABLE();
  gpio_init_structure.Pin = BUTTON_PIN [Button];
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Mode = GPIO_MODE_INPUT;    
    HAL_GPIO_Init(BUTTON_PORT [Button], &gpio_init_structure);
  }
  else if(ButtonMode == BUTTON_MODE_EXTI)
  {      
    /* Configure Button pin as input with External interrupt */    
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING; 
    
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
    
    HAL_EXTI_GetHandle(hExtiButtonHandle[Button], BUTTON_EXTI_LINE[Button]);  
    HAL_EXTI_RegisterCallback(hExtiButtonHandle[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);
      
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Push Button DeInit.
 * @param  Button Button to be configured
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_KEY: Key Push Button
 * @note PB DeInit does not disable the GPIO clock
 * @retval BSP status
 */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Returns the selected button state.
 * @param  Button Button to be checked
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_KEY: Key Push Button
 * @retval The Button GPIO pin value
 */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return (int32_t)HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
 * @brief  Key EXTI line detection callbacks.
 * @retval None
 */
static void BUTTON_KEY_EXTI_Callback(void)
{   
  BSP_PB_Callback(BUTTON_KEY);
}

/**
 * @brief  BSP Push Button callback
 * @param  Button Specifies the pin connected EXTI line
 * @retval None.
 */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);
  
  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */   
}
/**
 * @brief  Configures COM port.
 * @param  COM: COM port to be configured.
 *              This parameter can be COM1
 * @param  UART_Init: Pointer to a UART_HandleTypeDef structure that contains the
 *                    configuration information for the specified USART peripheral.
 * @retval BSP error code
 */
int32_t BSP_COM_Init(COM_TypeDef COM) 
{
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
  /* Init the UART Msp */
  USART2_MspInit(&hComHandle[COM]);
#else
  if(IsUsart2MspCbValid == 0U)
  {
    if(BSP_USART2_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
    {
      return BSP_ERROR_MSP_FAILURE;
    }
  }
#endif

  MX_USART2_UART_Init(&hComHandle[COM]);

  return BSP_ERROR_NONE;
}

/**
 * @brief  DeInit COM port.
 * @param  COM COM port to be configured.
 *             This parameter can be COM1
 * @retval BSP status
 */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  /* USART configuration */
  hComHandle[COM].Instance = COM_USART[COM];
  
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)  
  USART2_MspDeInit(&hComHandle[COM]);  
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */
  
  if(HAL_UART_DeInit(&hComHandle[COM]) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Configures COM port.
 * @param  huart USART handle
 *               This parameter can be COM1
 * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
 *                  configuration information for the specified USART peripheral.
 * @retval HAL error code
 */

/* USART2 init function */ 

__weak HAL_StatusTypeDef MX_USART2_UART_Init(UART_HandleTypeDef* huart)
{
  HAL_StatusTypeDef ret = HAL_OK;
  huart->Instance = USART2;
  huart->Init.BaudRate = 115200;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

/**
 * @brief  Initializes USART2 MSP.
 * @param  huart USART2 handle
 * @retval None
 */

static void USART2_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
}

static void USART2_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1) 
/**
 * @brief Register Default USART2 Bus Msp Callbacks
 * @retval BSP status
 */
int32_t BSP_USART2_RegisterDefaultMspCallbacks(void)
{
  int32_t ret = BSP_ERROR_NONE;
  
  __HAL_UART_RESET_HANDLE_STATE(&hComHandle[COM1]);
  
  /* Register default MspInit/MspDeInit Callback */
  if(HAL_UART_RegisterCallback(&hComHandle[COM1], HAL_UART_MSPINIT_CB_ID, USART2_MspInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_UART_RegisterCallback(&hComHandle[COM1], HAL_UART_MSPDEINIT_CB_ID, USART2_MspDeInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsUsart2MspCbValid = 1U;
  }
  
  /* BSP status */  
  return ret;
}

/**
 * @brief Register USART2 Bus Msp Callback registering
 * @param Callbacks pointer to USART2 MspInit/MspDeInit callback functions
 * @retval BSP status
 */
int32_t BSP_USART2_RegisterMspCallbacks (BSP_UART_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;
  
  __HAL_UART_RESET_HANDLE_STATE(&hComHandle[COM1]);
  
  /* Register MspInit/MspDeInit Callbacks */
  if(HAL_UART_RegisterCallback(&hComHandle[COM1], HAL_UART_MSPINIT_CB_ID, Callback->pMspUsartInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_UART_RegisterCallback(&hComHandle[COM1], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspUsartDeInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsUsart2MspCbValid = 1U;
  }
  
  /* BSP status */  
  return ret; 
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG == 1)
/**
 * @brief  Select the active COM port.
 * @param  COM COM port to be activated.
 *             This parameter can be COM1
 * @retval BSP status
 */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{ 
  if(COM_ActiveLogPort != COM)
  {
    COM_ActiveLogPort = COM;
  } 
  return BSP_ERROR_NONE; 
}

#ifdef __GNUC__ 
 int __io_putchar (int ch) 
#else 
 int fputc (int ch, FILE *f) 
#endif /* __GNUC__ */ 
{ 
  HAL_UART_Transmit(&hComHandle[COM_ActiveLogPort], (uint8_t *)&ch, 1, COM_POLL_TIMEOUT); 
  return ch;
}
#endif /* USE_COM_LOG */ 

/**
 * @}
 */ 

/**
 * @}
 */

/**
 * @}
 */    

/**
 * @}
 */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

