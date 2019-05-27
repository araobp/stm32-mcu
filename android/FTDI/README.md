# Testing Android FTDI driver

(Work in progress)

## Goal

I develop an oscilloscope GUI on Android for this project: https://github.com/araobp/acoustic-features

In this project, I will just confirm if Android can communicate with a NUCLEO board over UART/USB.

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
      |GND <--- GND| FTDI |----- USB OTG --| on Android
      |RX  <--- TX |      |                |
      |TX  <--- RX |      |                |
------+ |          +------+                +------
--------+
```

## UART-USB converter

I always use [this tiny converter](http://akizukidenshi.com/catalog/g/gM-08461/) from Akizuki Denshi in Akihabara, Tokyo.

## FTDI driver for Android

I already developed such a driver for Android based on [FTDI's D2XX driver](https://www.ftdichip.com/Android.htm), and used the driver in the projects below:
- https://github.com/araobp/sensor-network-android
- https://github.com/araobp/gps_android

I used to be a Java programmer, but I prefer Kotlin these days.

In this project, I will just develop a simple terminal application with the driver.
