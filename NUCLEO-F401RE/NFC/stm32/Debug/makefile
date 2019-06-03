################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/NFC04A1/subdir.mk
-include Drivers/BSP/Components/ST25DV/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Dynamic_NFC_tag.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Dynamic_NFC_tag.list \


# All Target
all: Dynamic_NFC_tag.elf secondary-outputs

# Tool invocations
Dynamic_NFC_tag.elf: $(OBJS) $(USER_OBJS) C:\Users\shiny\Documents\GitHub\stm32-mcu\NUCLEO-F401RE\NFC\stm32\STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "Dynamic_NFC_tag.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\shiny\Documents\GitHub\stm32-mcu\NUCLEO-F401RE\NFC\stm32\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Dynamic_NFC_tag.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Dynamic_NFC_tag.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Dynamic_NFC_tag.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
