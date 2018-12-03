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

### Constants #####

Fs = 80000000.0/25.0/42.0   # Sampling frequency
Nyq = Fs/2.0                # Nyquist frequency
BAUD_RATE = 115200          # UART baud rate
NN = 1024                   # The number of samples per frame

# main.c
RAW_WAVE = b'1'
COMPRESSION = b'2'

SCALE_DOWN=0.001

LEFT_MIC_ONLY = b'['
RIGHT_MIC_ONLY = b']'
BROADSIDE = b'b'
ENDFIRE = b'e'

# main.c
NUM_SAMPLES = {}            # The number of samples to receive from the device
NUM_SAMPLES[RAW_WAVE] = NN*4
NUM_SAMPLES[COMPRESSION] = NN*4

# Time axis and frequency axis
TIME = {}
FREQ = {}
TIME[RAW_WAVE] = np.linspace(0, NUM_SAMPLES[RAW_WAVE]/Fs*1000.0, NUM_SAMPLES[RAW_WAVE])

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
                if cmd == RAW_WAVE or cmd == COMPRESSION:  # 16bit quantization
                    rx = ser.read(NUM_SAMPLES[cmd]*2)
                    rx = zip(rx[0::2], rx[1::2])
                    for msb, lsb in rx:
                        n += 1
                        d =  int.from_bytes([msb, lsb], byteorder='big', signed=True)
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

    # Enable/disable beam forming
    def set_beam_forming(self, mode, angle):
        ser = serial.Serial(self.port, BAUD_RATE, timeout=3)
        ser.write(mode)
        ser.write(angle.encode('ascii'))
        ser.close()

    # Left mic only
    def left_mic_only(self):
        ser = serial.Serial(self.port, BAUD_RATE, timeout=3)
        ser.write(LEFT_MIC_ONLY)
        ser.close()

    # Right mic only
    def right_mic_only(self):
        ser = serial.Serial(self.port, BAUD_RATE, timeout=3)
        ser.write(RIGHT_MIC_ONLY)
        ser.close()

    # Use matplotlib to plot the output from the device
    def plot_aed(self, ax, cmd, range_=None, cmap=None, ssub=None):

        mag = self._serial_read(cmd, ssub)
        ax.clear()
        
        if cmd == RAW_WAVE:
            ax.set_title('Raw wave')
            ax.plot(TIME[RAW_WAVE], mag)
            ax.set_xlabel('Time [msec]')
            ax.set_ylabel('Amplitude')
            ax.set_ylim([-range_, range_])

        elif cmd == COMPRESSION:
            ax.set_title('Chirp compression')
            ax.plot(TIME[RAW_WAVE], mag)
            ax.set_xlabel('Time [msec]')
            ax.set_ylabel('Amplitude**2*{}'.format(SCALE_DOWN))
            ax.set_ylim([0, range_])

        return mag
