# HTTPS server for testing the device

## Architecture

```
[Local service function] generates URL
   [STM32F401RE]
        | I2C
        v
      [ST25](())[Chrome/Android] ----- HTTPS ----- [webapp.js]
                                                   + server.key
                                                   + sever.cert
```

## express.js over HTTPS

HTTPS is MUST to prevent fraud attacks.

Refer to the following link to run express.js over HTTPS:
https://timonweb.com/posts/running-expressjs-server-over-https/
