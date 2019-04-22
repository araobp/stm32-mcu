/**
  ******************************************************************************
  * @file  : nucleo_f401re.h
  * @brief : header file for the BSP Common driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
*/
 
				
				
				
				
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NUCLEO_F401RE_H
#define __NUCLEO_F401RE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/ 
#include "nucleo_f401re_conf.h"
#include "nucleo_f401re_errno.h"
#include "main.h"

#if (USE_BSP_COM_FEATURE > 0)
  #if (USE_COM_LOG > 0)
    #ifndef __GNUC__
      #include <stdio.h>
    #endif
  #endif
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup NUCLEO_F401RE
 * @{
 */

/** @addtogroup NUCLEO_F401RE_LOW_LEVEL
 * @{
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Exported_Types NUCLEO_F401RE LOW LEVEL Exported Types
 * @{
 */
 
 /** 
  * @brief Define for NUCLEO_F401RE board  
  */ 
#if !defined (USE_NUCLEO_F401RE)
 #define USE_NUCLEO_F401RE
#endif
#ifndef USE_BSP_COM_FEATURE
   #define USE_BSP_COM_FEATURE                  0U
#endif

#ifndef USE_BSP_COM
  #define USE_BSP_COM                           0U
#endif
 
#ifndef USE_COM_LOG
  #define USE_COM_LOG                           1U
#endif
  
#ifndef BSP_BUTTON_KEY_IT_PRIORITY
  #define BSP_BUTTON_KEY_IT_PRIORITY            15U
#endif 
  
typedef enum 
{
  LED2 = 0
} Led_TypeDef;

typedef enum 
{
  BUTTON_KEY = 0U
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum 
{
  COM1 = 0U,
}COM_TypeDef;

typedef enum
{          
 COM_STOPBITS_1     =   UART_STOPBITS_1,                                 
 COM_STOPBITS_2     =   UART_STOPBITS_2,
}COM_StopBitsTypeDef;

typedef enum
{
 COM_PARITY_NONE     =  UART_PARITY_NONE,                  
 COM_PARITY_EVEN     =  UART_PARITY_EVEN,                  
 COM_PARITY_ODD      =  UART_PARITY_ODD,                   
}COM_ParityTypeDef;

typedef enum
{
 COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,               
 COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,                
 COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,                
 COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS, 
}COM_HwFlowCtlTypeDef;

typedef struct
{
  uint32_t             BaudRate;      
  uint32_t             WordLength;    
  COM_StopBitsTypeDef  StopBits;      
  COM_ParityTypeDef    Parity;               
  COM_HwFlowCtlTypeDef HwFlowCtl;                           
}COM_InitTypeDef;
#endif

#define MX_UART_InitTypeDef          COM_InitTypeDef
#define MX_UART_StopBitsTypeDef      COM_StopBitsTypeDef
#define MX_UART_ParityTypeDef        COM_ParityTypeDef
#define MX_UART_HwFlowCtlTypeDef     COM_HwFlowCtlTypeDef

/**
 * @}
 */ 

/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_BUTTON NUCLEO_F401RE LOW LEVEL BUTTON
 * @{
 */ 
/* Button state */
#define BUTTON_RELEASED                   0U
#define BUTTON_PRESSED                    1U 

#define BUTTONn                           1U

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN	                  GPIO_PIN_13
#define KEY_BUTTON_GPIO_PORT              GPIOC
#define KEY_BUTTON_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()   
#define KEY_BUTTON_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()  
#define KEY_BUTTON_EXTI_IRQn              EXTI15_10_IRQn
#define KEY_BUTTON_EXTI_LINE              EXTI_LINE_13 

/**
 * @}
 */ 

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_COM NUCLEO_F401RE LOW LEVEL COM
 * @{
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

/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */   
extern EXTI_HandleTypeDef* hExtiButtonHandle[BUTTONn];
/**
  * @}
  */ 
    
/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Exported_Functions NUCLEO_F401RE LOW LEVEL Exported Functions
 * @{
 */ 

int32_t  BSP_GetVersion(void);  
int32_t  BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t  BSP_PB_DeInit(Button_TypeDef Button);
int32_t  BSP_PB_GetState(Button_TypeDef Button);
void     BSP_PB_Callback(Button_TypeDef Button);
void     BSP_PB_IRQHandler (Button_TypeDef Button);

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
#ifdef __cplusplus
}
#endif

#endif /* __NUCLEO_F401RE__H */
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
