# Application components for STMicro STM32 MCUs

Toolchain: [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

## Common libraries

### [NUCLEO-F401RE](http://www.st.com/en/evaluation-tools/nucleo-f401re.html) board

- [DCT Type-II implementation on CMSIS-DSP (also works on L4)](./NUCLEO-F401RE/DCT) including a zigzag scan utility function.

## Application components

### [NUCLEO-F401RE](http://www.st.com/en/evaluation-tools/nucleo-f401re.html) board

- [Thermography (infrared array sensor) and rock-paper-scissors recogniton with Keras/TensorFlow](./NUCLEO-F401RE/Thermography)
- [X-CUBE-AI (ML inference on MCU with 2D images from an infrared array sensor)](./NUCLEO-F401RE/AI)
- [X-CUBE-BLE1 (BLE)](./NUCLEO-F401RE/BLE)
- [Dynamic NFC tag](./NUCLEO-F401RE/NFC)

### [NUCLEO-L476RG](http://www.st.com/en/evaluation-tools/nucleo-l476rg.html) board

- [HelloWorld](./NUCLEO-L476RG/HelloWorld)
- [GPIO_Interrupt](./NUCLEO-L476RG/GPIO_Interrupt)
- [Clock](./NUCLEO-L476RG/Clock/README.md)
- [CAN](./NUCLEO-L476RG/CAN/README.md)
- [MEMSMIC(MEMS mic expansion board)](./NUCLEO-L476RG/MEMSMIC/README.md)
- [ADC with DMA](./NUCLEO-L476RG/ADC/README.md)
- [DAC with triangular wave and sine wave](./NUCLEO-L476RG/DAC/README.md)
- [Mic->ADC->DAC pipeline](./NUCLEO-L476RG/Mic_ADC_DAC/README.md)
- [Sound Effector](./NUCLEO-L476RG/Sound_Effector)
- [Chirp compression](./NUCLEO-L476RG/Chirp_Compression)

### [STM32F4 Discovery](http://www.st.com/en/evaluation-tools/stm32f4discovery.html) board

- [Camera (CMOS image sensor OV7670)](./STM32F4-Discovery/Camera)
- [BlueSwitch](./STM32F4-Discovery/BlueSwitch)
- [HelloWorld](./STM32F4-Discovery/HelloWorld)
- [LED_blinking](./STM32F4-Discovery/LED_blinking)
- [PWM-output](./STM32F4-Discovery/PWM-output)
- [AudioWeaver](./STM32F4-Discovery/AudioWeaver)

#### printf issue

I managed to support printf on USART2.

<img src="./doc/STM32F4_Discovery_UART.jpg" height=300>

Physical configuration:

```
STM32F407
               +------------+
            X 5V            |
GND     ----- GND  FTDI     |
PA3(RX) ----- TX  USB-UART  ===== USB ======> PC
PA2(TX) ----- RX  converter |
               +------------+
```

Include the following snippet in main.c to support printf:

```
int _write(int file, char *pbuf, int len)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)pbuf, len, 1000);
  return len;
}
```

#### PWM test

PWM output (100Hz, 50% duty) to PA8:

![waveform](./doc/PWM_waveform.jpg)

## Tips

- [CMSIS DSP RFFT pitfalls](./tips/CMSIS_DSP_RFFT.md)
- [Enabling DSP on STM32](./tips/ENABLE_DSP.md)
- [PCM data resolution on STM32L4 DFSDM](./tips/RESOLUTION.md)
- [Enabling printf on STM32](./tips/ENABLE_PRINTF.md)

## References

### STM32L476 Nucleo-64

- [Schematics](http://www.st.com/resource/en/schematic_pack/nucleo_64pins_sch.zip)
- [Datasheet](http://www.st.com/resource/en/datasheet/stm32l476je.pdf)

### STM32F4 Discovery board

- [Schematics](http://www.st.com/resource/en/schematic_pack/stm32f4discovery_sch.zip)
- [Datasheet](http://www.st.com/resource/en/datasheet/dm00037051.pdf)
