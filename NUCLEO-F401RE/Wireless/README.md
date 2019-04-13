# Wireless transport for IoT and edge AI

I use wireless communication modules supporting operations on UART for prototyping IoT and edge AI.

In this project, [a single page application (SPA)](./spa) is used for showing an inference result from an edge AI simulator.

## TWELITE

Although TWELITE is not so reliable, it is very cheap.

**==> [TWELITE](./TWELITE)**

## Bluetooth Low Energy

I use Microchip RN4020 that supports Bluetooth 4.x.  They say Bluetooth 5 seems to be the way to go for edge AI. 

**==> [BLE(Microchip RN4020)](./RN4020)**

#### Set up

```
[main.html]<-- mqtt over WebSocket --[mosquitto]<-- mqtt --[agent.py]<--[BLE module]<-- notify --[RN4020]<-- [edge_ai simulator.py]
Chrome browser                          RasPi                RasPi         RasPi                                     RasPi

```

## EnOcean

I used EnOcean just as a wireless switch to turn on LED.

## LoRa

I used LoRa for wireless communication in mountians.
