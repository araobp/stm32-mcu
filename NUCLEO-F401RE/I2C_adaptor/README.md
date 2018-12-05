# I2C adaptor for infrared sensor

I am going to develop a simple I2C-to-UART converter for infrared array sensors.

## Architecture

```  
                 +--------------------------+
                 |      +----------------+  |
                 |      |                |  |
               [R10k] [R10k]             |  |
                 |      |                |  |
                 |      |                |  |
                SCL    SDA               3.3V
           [Infrared sensor]---I2C--->[NUCLEO F401RE]--+--UART-+--->[heatmap.py/viewer.py/PC]
                GND   3.3V             3.3 GND         |       |
                 |      |                |  |          +--CAN--+
                 +      +----------------+  |          |       |
                 +--------------------------+          +--BLE--+

```

### Infrared array sensor (I2C slave)

- [Infrared array sensor (Panasonic AMG88XX)](https://industrial.panasonic.com/cdbs/www-data/pdf/ADI8000/ADI8000C53.pdf)
- [Data sheet](https://cdn-learn.adafruit.com/assets/assets/000/043/261/original/Grid-EYE_SPECIFICATIONS%28Reference%29.pdf?1498680225)

### Humidity and temperature sensor

- [Humidity and temperature sensor (Sensirion SHT32-DIS)](https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/)
- [Data sheet](http://www.mouser.com/ds/2/682/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital-971521.pdf)

## Schematic of my original Arduino shield

- [Arduino shield](./kicad/arduino_board.pdf)

## Capabilities to be developed

- Read registors successively.
- Timer to read registors periodically (e.g., 100msec)
- 8bit binary data output to UART (to the viewer)
- Data output to CAN and BLE (to the viewer)
- Viewer based on matplotlib/Tkinter
- Pre-processing for training RNN model on Keras/TensorFlow

## Implementation at the moment

- [I2C adaptor on NUCLEO F401RE](./stm32)
- [viewer](./viewer)
