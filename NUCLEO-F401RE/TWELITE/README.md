# TWELITE

(Work in progress)

TWELITE is a IEEE802.15.4-based sensor networking technology by a Japanese company [MONO WIRELESS](https://mono-wireless.com/en/).

I used TWELITE with PIC16F1 MCU in a very important IoT projects in Feb 2019. It was very successful, so I will use TWELITE rather than other low-power wireless technologies such as BLE, in my upcoming projects for the time being.

## Goal

Port this code to STM32: https://github.com/araobp/pic16f1-mcu/blob/master/src/pic16f18326/amg8833.X/twelite.c

## Set up

### NUCLEO board

SB62 and SB63 need to be shorted to transmit data to D1 or receive data from D0.

<img src="./doc/soldering_bridges.jpg" width=250>

### Circuit

TWELITE-DIP is powered by 3V3 pin on the NUCLEO board:
```
 NUCLEO      TWELITE
  3V3 ------- VCC
  GND ------- GND
  TX/D1 ----- RX
  RX/D0 ----- TX

```

<img src="./doc/jumper_cables.jpg" width=400>

**=> [TWELITE DIP pin assignment](https://mono-wireless.com/jp/products/TWE-APPS/App_Twelite/asset/twe_app_pins.png)**

## Code

I use TWELITE's Binary Transfer Mode to transfer data in a bulk to an edge AI application.

- [Slave node: TWELITE driver and test program for STM32](./stm32)
- [Master node: TWELITE driver and test program for Python](./python)
