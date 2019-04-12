import serial
import time

PORT = 'COM13'
BAUDRATE = 115200

CHARACTERISTIC_UUID = "010203040506070809000A0B0C0D0E0F"

def notify(ser, inference_result):
    ser.write("SUW,{},{:02x}\n".format(CHARACTERISTIC_UUID, inference_result).encode('ascii'))

classes = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
with serial.Serial(PORT, BAUDRATE) as ser:
    i = 0
    #ser.write("A\n".encode('ascii'))
    time.sleep(1)
    while True:
        notify(ser, classes[i])
        print("{:02x}".format(classes[i]))
        i += 1
        if i > 15:
            i = 0
        time.sleep(1)
