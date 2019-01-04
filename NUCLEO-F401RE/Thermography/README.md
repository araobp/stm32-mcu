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

![](./device.jpg)

- [Arduino shield](./kicad/arduino_board.pdf)

## Implementation

- [I2C adaptor on NUCLEO F401RE](./stm32)
- [Thermography GUI](./thermography)

## Deep learning on rock-paper-scissors with Keras/TensorFlow

### Training CNN model

Class labels:
- rock
- paper
- scissors

Training data: 2D array of 8 x 8 pixels or 32 x 32 pixels (8 x 8 images interpolated)

Data set: [csv files](./thermography/data)

Training result:
- [8 x 8 2D images](./tensorflow/CNN_for_rock_paper_scissors.ipynb)
- [32 x 32 2D images](./tensorflow/CNN_for_rock_paper_scissors_interpolated.ipynb)

### Using the trained CNN model

![](./thermography/screenshots/ml_rock.jpg)

![](./thermography/screenshots/ml_paper.jpg)

![](./thermography/screenshots/ml_scissors.jpg)

## DCT

Descrete Cosine Transform of 2D image shows some features. I am going to use DCT output as an additional feature for training a neural network.

![](./thermography/screenshots/dct.jpg)
