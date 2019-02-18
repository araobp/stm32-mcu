
import numpy as np
import matplotlib.pyplot as plt
import math

import interface

class GUI:

    def __init__(self, interface):
        self.interface = interface
        
    # Use matplotlib to plot the output from the device
    def plot(self, ax, cmd, cmap=None, ssub=None):

        data = self.interface.read(cmd)
        
        if cmd == interface.PIXELS:
            ax.set_title('Image')
            ax.imshow(data)              
    
        return data
