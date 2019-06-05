# Home appliances

## Home Digital Signage

I use TV in my living room as "Home Digital Signage". I use RasPi as a digital signage controller with ST25DV and SONY TV.

The controller send MQTT messages to repeatedly show muliple images at some interval on the display (display.html), and the contents on the display are synchronized with URLs on ST25DV04K.

```
  [signage_controller.py] -- MQTT --> [mosquitto] -- MQTT/WebSocket --> [display.html]
             | UART
             v
       [STM32F401RE]
             | I2C
             v
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----> [webapp.js, WWW and YouTube]

```

## Video player

This is just to rewind video playback to the begining.

The player just playbacks a sample video content "Yokohama Osanbashi" on Chrome for Android.

```
                                                                       Tablet PC (Android)
         [gateway.py] -- MQTT --> [mosquitto] -- MQTT/WebSocket --> [yokohama_osanbashi.html]
     [RasPi's onboard BLE]                                               [WebBluetooth]
             ^                                                                   ^
             |                                                                   |
         (case 2)                                                            (case 1)
             | BLE                                                               | BLE
         [RN4020]----------------------------------------------------------------+
             ^
             | UART
       [STM32F401RE]
             | RF field change event
            GPO
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----- [WWW or YouTube]

Case 1: RF field change event is received by Web Bluetooth.
Case 2: RF field change event is received by gateway.py, and it transfers the event to the html page.

```
