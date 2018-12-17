#
# Heatmap
#
# Reference: https://learn.adafruit.com/adafruit-amg8833-8x8-thermal-camera-sensor/raspberry-pi-thermal-camera
#

import serial
import pandas as pd
import numpy as np
import math
import traceback
import threading
#import matplotlib.pyplot as plt
import seaborn as sns; sns.set()
from scipy.interpolate import griddata
from scipy.fftpack import dct, idct


### Constants #####

BAUD_RATE = 460800          # UART baud rate

PIXELS = b'p'
THERMISTOR = b't'

# main.c
NUM_SAMPLES = {}            # The number of samples to receive from the device
NUM_SAMPLES[PIXELS] = 64
NUM_SAMPLES[THERMISTOR] = 1

# Points and grids for interpolation of 2d image
POINTS = [(math.floor(n / 8), (n % 8)) for n in range(0, 64)]
GRID_X, GRID_Y = np.mgrid[0:7:32j, 0:7:32j]

import sklearn.preprocessing as pp

def dct_2d(image):
  return dct(dct(image.T, norm='ortho').T, norm='ortho')

def idct_2d(coef):
  return idct(idct(coef.T, norm='ortho').T, norm='ortho')

###################

# GUI class
class GUI:
    
    def __init__(self, port, grid_data):
        # Serial interface
        self.port = port
        self.grid_data=grid_data
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

        data = self.read(cmd)
        
        if cmd == PIXELS:
            data = data * 0.25  # Resolution: 0.25 per degress Celsius
            if self.grid_data:
                data = griddata(POINTS, data, (GRID_X, GRID_Y), method='cubic')
                data = np.flip(np.flip(data.reshape(32,32), axis=0), axis=1)
            else:
                data = np.flip(np.flip(data.reshape(8,8), axis=0), axis=1)
                
            axes[0].set_title('Heat map')
            sns.heatmap(data, cmap=cmap, ax=axes[0], cbar_ax=axes[1])
            axes[2].set_title('DCT')
            coef = dct_2d(data)
            coef[0,0] = 0  # Remove DC
            sns.heatmap(np.abs(coef), cmap='gray', vmin=0, vmax=10, ax=axes[2], cbar_ax=axes[3])

        elif cmd == THERMISTOR:
            data = (data - 20) * 0.0625  # Resolution: 0.0625 per degree Celsius
            
        return data
