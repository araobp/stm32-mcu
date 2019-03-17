# Testing Android FTDI driver

(Work in progress)

## Set up

I dare not use ST-Link USB port on NUCLEO board in this project, because the debugger part should be removed to make the board more portable.

I will make an Arduino shield with the following pins:

```

NUCLEO board
--------+
        |
Arduino shield    UART-USB converter
------+            +------+                +------
      |E5V <--- 5V |      |                | Terminal application
      |GND <--- GND| FTDI |----- USB ------| on Android
      |RX  <--- TX |      |                |
      |TX  <--- RX |      |                |
------+ |          +------+                +------
--------+
```

## UART-USB converter

I always use [this tiny converter](http://akizukidenshi.com/catalog/g/gM-08461/) from Akizuki Denshi in Akihabara, Tokyo.

## FTDI driver for Android

I already developed such a driver for Android with this library: [D2XX driver](https://www.ftdichip.com/Android.htm)

I used to be a Java programmer, but I prefer Kotlin these days.

In this project, I will just develop a simple terminal application with the driver.
