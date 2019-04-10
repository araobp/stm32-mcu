import numpy as np

b16_to_int = lambda msb, lsb, signed: int.from_bytes([msb, lsb], byteorder='little', signed=signed)
b8_to_int = lambda d, signed: int.from_bytes([d], byteorder='little', signed=signed)

def to_int16_array(data):
    '''
    convert binary object into int16_t array
    '''
    tmp = []
    data = zip(data[0::2], data[1::2])
    for msb, lsb in data:
        d = b16_to_int(msb, lsb, True)
        tmp.append(d)
    tmp = np.array(tmp, dtype=np.int16)
    return tmp

def to_int8_array(data):
    '''
    convert binary object into int8_t array
    '''
    tmp = []
    for lsb in data:
        d = b8_to_int(lsb, True)
        tmp.append(d)
    tmp = np.array(tmp, dtype=np.int8)
    return tmp

