# Thermography

![](./thermography/screenshots/this_is_me.png)

The picture above is me on thermography GUI that I have developed based on matplotlib with Tk.

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

## Infrared array sensor (I2C slave)

- [Infrared array sensor (Panasonic AMG88XX)](https://industrial.panasonic.com/cdbs/www-data/pdf/ADI8000/ADI8000C53.pdf)
- [Data sheet](https://cdn-learn.adafruit.com/assets/assets/000/043/261/original/Grid-EYE_SPECIFICATIONS%28Reference%29.pdf?1498680225)

## Schematic of my original Arduino shield

- [Arduino shield](./kicad/arduino_board.pdf)

## Implementation at the moment

- [I2C adaptor on NUCLEO F401RE](./stm32)
- [Thermography GUI](./thermography)

## Deep learning experiment

### Rock-paper-scissors recognition

Class labels:
- rock
- paper
- scissors

Training data: 2D array of 8 x 8 pixels

