import serial
import numpy as np
import traceback

### Constants #####

BAUD_RATE = 460800          # UART baud rate

PIXELS = b'p'
BRIGHTNESS = b'b'
CONTRAST = b'c'

QCIF_WIDTH = 172
QCIF_HEIGHT = 144

NUM_SAMPLES = {"q": QCIF_WIDTH * QCIF_HEIGHT * 2,
               "m": 128 * 128 * 2,
               "s": 32 * 32 * 2}

SHAPE = {"q": (QCIF_HEIGHT, QCIF_WIDTH, 3),
         "m": (128, 128, 3),
         "s": (32, 32, 3)}

class Interface:
    
    def __init__(self, port, size):
        # Serial interface
        self.port = port
        self.size = size
        self.ser = None
        try:
            self.ser = serial.Serial(self.port, BAUD_RATE, timeout=3)
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
        n = 0
        try:
            self.ser.write(cmd+b'\n')
            print(cmd)
            rx = self.ser.read(NUM_SAMPLES[self.size])
            #print(rx)
            rx = zip(rx[0::2], rx[1::2])
            for lsb, msb in rx:
                n += 1
                d =  int.from_bytes([msb, lsb], byteorder='big', signed=False)
                red =   (d & 0b1111100000000000) >> 11
                green = (d & 0b0000011111100000) >> 5
                blue =   d & 0b0000000000011111
                data.append((red << 3, green << 2, blue << 3))
                #print("{:02x} {:02x} {:02x}".format(red, green, blue))
            data = np.array(data, dtype=np.uint8).reshape(*SHAPE[self.size])
            print(data.shape)
        except:
            print('*** serial timeout!')
            traceback.print_exc()

        return data
    

    def write(self, cmd, value):
            try:
                cmd_line = cmd + bytes(str(value), encoding='ascii') + b'\n'
                self.ser.write(cmd_line)
                print(cmd_line)
            except:
                traceback.print_exc()

