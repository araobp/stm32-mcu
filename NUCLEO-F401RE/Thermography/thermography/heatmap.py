#
# Heatmap
#
# Reference: https://learn.adafruit.com/adafruit-amg8833-8x8-thermal-camera-sensor/raspberry-pi-thermal-camera
#

import numpy as np
import math
import seaborn as sns; sns.set()
from scipy.interpolate import griddata
from scipy.fftpack import dct, idct

import interface

### Constants #####

# Points and grids for interpolation of 2d image
POINTS = [(math.floor(n / 8), (n % 8)) for n in range(0, 64)]
GRID_X, GRID_Y = np.mgrid[0:7:32j, 0:7:32j]

import sklearn.preprocessing as pp

def dct_2d(image):
  return dct(dct(image.T, norm='ortho').T, norm='ortho')

def idct_2d(coef):
  return idct(idct(coef.T, norm='ortho').T, norm='ortho')

###################

class GUI:

    def __init__(self, interface, grid_data):
        self.interface = interface
        self.grid_data = grid_data
        
    # Use matplotlib to plot the output from the device
    def plot(self, axes, cmd, cmap=None, ssub=None):

        data = self.interface.read(cmd)
        
        if cmd == interface.PIXELS:
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

        elif cmd == interface.THERMISTOR:
            data = (data - 20) * 0.0625  # Resolution: 0.0625 per degree Celsius
            
        return data
