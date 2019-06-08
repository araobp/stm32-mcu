# Home appliances

## Home Digital Signage

I use TV in my living room as "Home Digital Signage". I use RasPi as a digital signage controller with ST25DV and SONY TV.

display.html shows some contents every 10 seconds repeatedly, and sends POST request to webapp.js everytime new content is shown on the screen. webapp.js receives the POST request and writes a new URL onto ST25's EEPROM.

```
        [webapp.js] <---- POST ----------------------------- [display.html]
             | UART
             v
       [STM32F401RE]
             | I2C
             v
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----> [WWW and YouTube]

```

## Video player

This is just to rewind video playback to the begining.

The player just playbacks a sample video content "Yokohama Osanbashi" on Chrome for Android.

```
     Tablet PC (Android)
   [yokohama_osanbashi.html]
             ^
             | BLE (RF field change event is received by Web Bluetooth)
         [RN4020]
             ^
             | UART
       [STM32F401RE]
             | RF field change event
            GPO
           [ST25](())[Chrome/Android] ----- HTTP or HTTPS ----- [WWW or YouTube]
```
