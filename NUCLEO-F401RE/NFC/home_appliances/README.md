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
   [yokohama_osanbashi.html]
             ^
             | BLE
         [RN4020]
             ^
             | UART
       [STM32F401RE]
             | RF field change event
            GPO
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----- [WWW or YouTube]

Case 1: RF field change event is received by Web Bluetooth.
Case 2: RF field change event is received by gateway.py, and it transfers the event to the html page.

```

## MQTTS

Web Bluetooth and some other advanced features are allowd to run in secure contexts only, so mosquitto MQTT broker needs to open MQTT over WebSockets over SSL/TLS as well.

### Set up 

- [MQTT Mosquitto broker with SSL/TLS transport security](https://primalcortex.wordpress.com/2016/03/31/mqtt-mosquitto-broker-with-ssltls-transport-security/)
- [generate-CA.sh](https://raw.githubusercontent.com/owntracks/tools/master/TLS/generate-CA.sh)

### Dealing with ERR_CERT_AUTHORITY_INVALID in Chrome

I faced a problem of ERR_CERT_AUTHORITY_INVALID output to the developer's console.

My solution for that is to access the websocket port on mosquitto by typing the following URL in the address bar:

```
https://<host address>:<wss port number>/
```

Then acknowledge the access on the web page. After that, the mqtt client connects to wss URL with no problems.

