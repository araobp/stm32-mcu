# CAN

The program runs in Loopback/Silent mode. Connect Teraterm to the board at baudrate 9600bps to show output messages.

```
[1] MailBox#: 1
[2] MailBox#: 1
[1] StdId: 1
[1] RX_FIFO0: Hello!
[2] StdId: 2
[2] RX_FIFO1: World!
       :
```

## CubeMX bug

My program did not work after rebooting the Nucleo board.
The debugger showed me that the system stalls at SystemClock_Config().

The cause was a bug in CubeMX: https://community.st.com/thread/43671-cubemx-4221-bug

```
RCC_OscInitStruct.PLL.PLLM = 1;
```

It was required to upgrading CubeMX from V4.25 to V4.25.1 to fixed the bug.

```
STM32CubeMX Software V4.25.1 / 05 May 2018 Minor release 
Fixed PLLM code generation for STM32L4 Series.
```

It was not a minor issue for me, because I spent so much time for the problem to fix.
