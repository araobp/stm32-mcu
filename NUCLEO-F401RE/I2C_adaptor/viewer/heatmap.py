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
import matplotlib.pyplot as plt

### Constants #####

BAUD_RATE = 115200          # UART baud rate
NN = 64                     # The number of samples per frame

PIXELS = b't'

# main.c
NUM_SAMPLES = {}            # The number of samples to receive from the device
NUM_SAMPLES[PIXELS] = NN

###################

# GUI class
class GUI:
    
    def __init__(self, port):
        # Serial interface
        self.port = port
        self.lock = threading.Lock()

    # As an application processor, send a command
    # then receive and process the output.
    def _serial_read(self, cmd, ssub=None):

        data = []
        with self.lock:
            n = 0
            try:
                ser = serial.Serial(self.port, BAUD_RATE, timeout=3, inter_byte_timeout=3)
                ser.write(cmd)
                if cmd == PIXELS:  # 16bit quantization
                    rx = ser.read(NUM_SAMPLES[cmd]*2)
                    rx = zip(rx[0::2], rx[1::2])
                    for lsb, msb in rx:
                        n += 1
                        d =  int.from_bytes([msb, lsb], byteorder='big', signed=False) * 0.25
                        data.append(d)
                    data = np.array(data, dtype=np.int16)
                else:  # 8bit quantization
                    rx = ser.read(NUM_SAMPLES[cmd])
                    for d in rx:
                        n += 1
                        d =  int.from_bytes([d], byteorder='big', signed=True)
                        if ssub and (ssub > 0):
                            d = d - ssub
                            if d < 0:
                                d = 0.0
                        data.append(d)
                    data = np.array(data, dtype=np.int8)
                ser.close()
            except:
                print('*** serial timeout!')
                traceback.print_exc()

        return data

    # Use matplotlib to plot the output from the device
    def plot_temp(self, ax, cmd, cmap=None, ssub=None):

        mag = self._serial_read(cmd, ssub)
        ax.clear()
        
        if cmd == PIXELS:
            mag = np.flip(np.flip(mag.reshape(8,8), axis=0), axis=1)
            pos = ax.imshow(mag, cmap=cmap)

        return mag
