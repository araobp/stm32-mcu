import numpy as np

b16_to_int = lambda msb, lsb, signed: int.from_bytes([msb, lsb], byteorder='big', signed=signed)
b8_to_int = lambda d, signed: int.from_bytes([d], byteorder='big', signed=signed)

def to_int16_array(data):
    tmp = []
    data = zip(data[0::2], data[1::2])
    for msb, lsb in data:
        d = b16_to_int(msb, lsb, True)
        tmp.append(d)
    tmp = np.array(tmp, dtype=np.int16)
    return tmp

def to_int8_array(data):
    tmp = []
    for lsb in data:
        d = b8_to_int(lsb, True)
        tmp.append(d)
    tmp = np.array(tmp, dtype=np.int8)
    return tmp

