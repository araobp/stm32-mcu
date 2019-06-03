################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.c \
../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.c 

OBJS += \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.o \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.o 

C_DEPS += \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.d \
./Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/lib_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.o: ../Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I..//Drivers/BSP/NFC04A1 -I../Inc -I..//Middlewares/ST/lib_nfc/common/inc -I..//Middlewares/ST/lib_nfc/lib_NDEF/Core/inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I..//Drivers/BSP/Components/ST25DV -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

