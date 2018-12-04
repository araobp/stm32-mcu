# I2C adaptor

I am going to develop a simple I2C-to-UART converter for infrared array sensors etc.

## Architecture

```  
                 +--------------------------+
                 |      +----------------+  |
                 |      |                |  |
               [R10k] [R10k]             |  |
                 |      |                |  |
 +---------------+      |                |  |
 |       +--------------+                |  |
 |       |       |      |                |  |
SCL     SDA     SCL    SDA               3.3V
[I2C slave]     [I2C slave]----I2C--->[NUCLEO F401RE]--+--UART-+--->[heatmap.py/viewer.py/PC]
GND    3.3V     GND   3.3V             3.3 GND         |       |
 |       |       |      |                |  |          +--CAN--+
 |       +--------------+----------------+  |          |       |
 +---------------+--------------------------+          +--BLE--+
 
```

## I2C devices

- [Humidity and temperature sensor (Sensirion SHT32-DIS)](http://akizukidenshi.com/catalog/g/gK-12125/)
- [Infrared array sensor (Panasonic AMG8833)](https://eu.industrial.panasonic.com/products/sensors-optical-devices/sensors-automotive-and-industrial-applications/infrared-array/series/grid-eye-high-performance-type-amg8833/ADI8005)

## Capabilities in development

- Read registors successively.
- Timer to read registors periodically (e.g., 100msec)
- 8bit binary data output to UART (to the viewer)
- Data output to CAN and BLE (to the viewer)

## Implementation

- [I2C adaptor on NUCLEO F401RE](./stm32)
- [viewer](./viewer)
