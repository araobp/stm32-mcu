# << Digial signal processing for oscilloscope GUI >>
#
# (1) Interface to the edge device (STM32L4 w/ my original
#     MEMS mic Arduino shield)
# (2) Plot the data with matplotlib
#
# Follow the definition in the include files below:
# https://github.com/araobp/acoustic-event-detection/tree/master/stm32/Inc
#

import serial
import pandas as pd
import numpy as np
import traceback
import threading
#import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

### Constants #####

BAUD_RATE = 460800          # UART baud rate

PIXELS = b'p'
THERMISTOR = b't'

# main.c
NUM_SAMPLES = {}            # The number of samples to receive from the device
NUM_SAMPLES[PIXELS] = 64
NUM_SAMPLES[THERMISTOR] = 1

###################

# GUI class
class GUI:
    
    def __init__(self, port):
        # Serial interface
        self.port = port
        self.lock = threading.Lock()
        self.ser = None
        try:
            self.ser = serial.Serial(self.port, BAUD_RATE, timeout=3, inter_byte_timeout=3)
        except:
            print('*** Serial connection failure!')
            traceback.print_exc()

    def close(self):
        if self.ser:
            self.ser.close()
                
    # As an application processor, send a command
    # then receive and process the output.
    def read(self, cmd, ssub=None):

        data = []
        with self.lock:
            n = 0
            try:
                self.ser.write(cmd)
                if cmd == PIXELS or cmd == THERMISTOR:  # 16bit quantization
                    rx = self.ser.read(NUM_SAMPLES[cmd]*2)
                    rx = zip(rx[0::2], rx[1::2])
                    for lsb, msb in rx:
                        n += 1
                        d =  int.from_bytes([msb, lsb], byteorder='big', signed=False)
                        data.append(d)
                    data = np.array(data, dtype=np.int16)
                else:  # 8bit quantization
                    rx = self.ser.PIXELS(NUM_SAMPLES[cmd])
                    for d in rx:
                        n += 1
                        d =  int.from_bytes([d], byteorder='big', signed=True)
                        if ssub and (ssub > 0):
                            d = d - ssub
                            if d < 0:
                                d = 0.0
                        data.append(d)
                    data = np.array(data, dtype=np.int8)
            except:
                print('*** serial timeout!')
                traceback.print_exc()

        return data

    # Use matplotlib to plot the output from the device
    def plot(self, axes, cmd, cmap=None, ssub=None):

        mag = self.read(cmd)
        
        if cmd == PIXELS:
            mag = mag * 0.25  # Resolution: 0.25 per degress Celsius
            mag = np.flip(np.flip(mag.reshape(8,8), axis=0), axis=1)
            axes[0].set_title('Heat map')
            sns.heatmap(mag, cmap=cmap, ax=axes[0], cbar_ax=axes[1])

        elif cmd == THERMISTOR:
            mag = (mag - 20) * 0.0625  # Resolution: 0.0625 per degree Celsius
            
        return mag
