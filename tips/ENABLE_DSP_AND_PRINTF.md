# Enabling DSP on STM32

[Step 1] Create lib directory as follows:

```
.\audio-signal-processing\dac\sine_wave\Drivers\CMSIS\Lib
```

[Step 2] Copy "libarm_cortexM4lf_math.a" and "LICENSE" to the directory

[Step 3] Add the library Paths "Drivers/CMSIS/Lib"

```
C/C++ General -> Path and Symbols -> Library Paths
```

[Step 4] Add the library "arm_cortexM4lf_math"


```
C/C++ General -> Path and Symbols -> Libraries
```
[Step 5] Add "__FPU_PRESENT=1" and "ARM_MATH_CM4"

```
C/C++ General -> Path and Symbols -> Symbols
```

# Enabling printf on STM32

[Step 1] Include the following snippet in main.c

```
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *) ptr, (uint16_t) len, 0xFFFFFFFF);
  return len;
}
```

[Step 2 (optional)] Enable floating point format by adding this option "-u _printf_float"

```
C/C++ Build -> Settings -> Toos settings -> C Linker -> Miscellanious
```

[Step 3] Include "stdio.h"
