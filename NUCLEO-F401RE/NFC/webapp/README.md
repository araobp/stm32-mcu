# Web application servers

## Web app server with dynamic NFC tag

### Architecture

```
[Local service function] generates URL
   [STM32F401RE]
        | I2C
        v
      [ST25](())[Chrome/Android] ----- HTTPS over WiFi ----- [webapp.js]
                                                             + server.key
                                                             + sever.cert
```

### express.js over HTTPS

HTTPS is MUST to prevent fraud attacks.

Refer to the following link to run express.js over HTTPS:
https://timonweb.com/posts/running-expressjs-server-over-https/

## Digital signage with dynamic NFC tag

This is to make RasPi as a digital signage controller with dynamic NFC tag.

The controller send MQTT messages to repeatedly show muliple images at some interval on the display (display.html), and the contents on the display are synchronized with URLs on ST25DV04K.

```
  [signage_controller.py] -- MQTT --> [mosquitto] -- MQTT/WebSocket --> [index.html]
             | UART
             v
       [STM32F401RE]
             | I2C
             v
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----- [WWW or YouTube]

```
