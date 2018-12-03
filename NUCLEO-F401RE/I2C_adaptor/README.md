# I2C adaptor

I am going to develop a simple I2C-to-UART converter.

## Architecture

```  
     +--------------------------+
     |      +----------------+  |
     |      |                |  |
   [R10k] [R10k]             |  |
     |      |                |  |
    SCL    SDA               3.3V
    [I2C slave]----I2C--->[NUCLEO F401RE]----UART--->[i2c.py/PC]
    GND   3.3V             3.3 GND
     |      |                |  |
     |      +----------------+  |
     +--------------------------+
```

## Features

- Read registors successively.
- Timer to read registors periodically (e.g., 100msec)
- 8bit binary data output to UART

## Test

i2c.py
