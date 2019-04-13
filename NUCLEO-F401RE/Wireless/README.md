# Wireless transport for IoT and edge AI

I use wireless communication modules supporting operations on UART for prototyping IoT and edge AI.

In this project, [a single page application (SPA)](./spa) is used for showing an inference result from an edge AI simulator.

## TWELITE

Although TWELITE is not so reliable, it is very cheap.

**==> [TWELITE](./TWELITE)**

## Bluetooth Low Energy

They say Bluetooth 5 seems to be the way to go for edge AI.

I saw several BLE-WiFi router products at a trade show held in Tokyo in April 2019. I am quite interested in them, so I develop a prototype of such a router.

I use Microchip RN4020 that supports Bluetooth 4.x.  

agent.py works as BLE-mqtt/WiFi router.

**==> [BLE(Microchip RN4020)](./RN4020)**

#### Set up: edge AI simulator

```
[main.html]<-- mqtt/WebSocket --[mosquitto]<-- mqtt --[agent.py]<--[BLE module]<-- notify --[RN4020]<--[edge_ai simulator.py]
Chrome browser                     RasPi                RasPi         RasPi                                     RasPi
```

#### Set up: RN4020 sheild on NUCLEO-F401RE

```
[main.html]<-- mqtt/WebSocket --[mosquitto]<-- mqtt --[agent.py]<--[BLE module]<-- notify --[RN4020]<--[edge AI simulator]
Chrome browser                     RasPi                RasPi         RasPi                  Shield      NUCLEO-F401RE    
```

## EnOcean

I used EnOcean just as a wireless switch to turn on LED.

## LoRa

I used LoRa for wireless communication in mountians.
