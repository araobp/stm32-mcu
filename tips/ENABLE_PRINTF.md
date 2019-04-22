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
