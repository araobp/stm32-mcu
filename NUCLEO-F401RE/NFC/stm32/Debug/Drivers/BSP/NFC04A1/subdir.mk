################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/NFC04A1/nfc04a1.c \
../Drivers/BSP/NFC04A1/nfc04a1_nfctag.c 

OBJS += \
./Drivers/BSP/NFC04A1/nfc04a1.o \
./Drivers/BSP/NFC04A1/nfc04a1_nfctag.o 

C_DEPS += \
./Drivers/BSP/NFC04A1/nfc04a1.d \
./Drivers/BSP/NFC04A1/nfc04a1_nfctag.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/NFC04A1/nfc04a1.o: ../Drivers/BSP/NFC04A1/nfc04a1.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/NFC04A1/nfc04a1.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/NFC04A1/nfc04a1_nfctag.o: ../Drivers/BSP/NFC04A1/nfc04a1_nfctag.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/NFC04A1/nfc04a1_nfctag.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

