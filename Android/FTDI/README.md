# Testing Android FTDI driver

## Set up

```
STM32           UART-USB converter
------+            +------+                +------
      |E5V <--- 5V |      |                |
      |GND <--- GND| FTDI |----- USB ------| Android
      |RX  <--- TX |      |                |
      |TX  <--- RX |      |                |
------+            +------+                +------
```
(Work in progress)

## FTDI driver for Android

- [D2XX driver](https://www.ftdichip.com/Android.htm)
