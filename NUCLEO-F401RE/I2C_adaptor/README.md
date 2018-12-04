# I2C adaptor

I am going to develop a simple I2C-to-UART converter for infrared array sensors etc.

## Architecture

```  
     +--------------------------+
     |      +----------------+  |
     |      |                |  |
   [R10k] [R10k]             |  |
     |      |                |  |
    SCL    SDA               3.3V
    [I2C slave]----I2C--->[NUCLEO F401RE]----UART--->[heatmap.py/viewer.py/PC]
    GND   3.3V             3.3 GND
     |      |                |  |
     |      +----------------+  |
     +--------------------------+
```

## Capabilities in development

- Read registors successively.
- Timer to read registors periodically (e.g., 100msec)
- 8bit binary data output to UART (to the viewer)
- Data output to BLE (to the viewer)

## Implementation

- [I2C adaptor on NUCLEO F401RE](./stm32)
- [viewer](./viewer)
