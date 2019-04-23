/**
  ******************************************************************************
  * @file           : nucleo_f401re_bus.h
  * @brief          : header file for the BSP BUS IO driver
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
#ifndef NUCLEO_F401RE_BUS_H
#define NUCLEO_F401RE_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nucleo_f401re_conf.h"
#include "nucleo_f401re_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup NUCLEO_F401RE
  * @{
  */

/** @defgroup NUCLEO_F401RE_BUS NUCLEO_F401RE BUS
  * @{
  */

/** @defgroup NUCLEO_F401RE_BUS_Exported_Constants NUCLEO_F401RE BUS Exported Constants
  * @{
  */

#ifndef BUS_SPI1_POLL_TIMEOUT
  #define BUS_SPI1_POLL_TIMEOUT                   0x1000
#endif
/* SPI1 Baud rate in bps  */
#ifndef BUS_SPI1_BAUDRATE   
   #define BUS_SPI1_BAUDRATE   10000000U /* baud rate of SPIn = 10 Mbps*/
#endif

/**
  * @}
  */

/** @defgroup NUCLEO_F401RE_BUS_Private_Types NUCLEO_F401RE BUS Private types
  * @{
  */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1) 
typedef struct
{
  pSPI_CallbackTypeDef  pMspInitCb;
  pSPI_CallbackTypeDef  pMspDeInitCb;
}BSP_SPI1_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */
  
/** @defgroup NUCLEO_F401RE_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */ 
extern SPI_HandleTypeDef hspi1;	
/**
  * @}
  */

/** @addtogroup NUCLEO_F401RE_BUS_Exported_Functions
  * @{
  */    

/* BUS IO driver over SPI Peripheral */
HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef* hspi);
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);

int32_t BSP_GetTick(void);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI1_RegisterMspCallbacks (BSP_SPI1_Cb_t *Callbacks);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI1_RegisterMspCallbacks (BSP_SPI1_Cb_t *Callbacks);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1)  */

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

#endif /* NUCLEO_F401RE_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
