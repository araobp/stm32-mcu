import serial

### Byte to integer conversion (2's complement)
b2ui = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=False)
b2i = lambda data, idx: int.from_bytes([data[idx]], byteorder='big', signed=True)

### TWELITE packet structure
BYTE = 0xA0
EOT = 0x04
# Options
RESPONSE_MSG_DISABLED = 0x07
ACK_ENABLED = 0x01
RESEND = 0x02
TERMINATOR = 0xFF
# The number of resend in case of communication error
NUM_RETRY = 3

## Serial read timeout
TIMEOUT = 3  # 3sec

# LQI to dBm conversion
def lqi2dbm(lqi):
    '''
    Convert LQI into dBm
    '''
    return (7.0 * lqi - 1970) / 20.0

class GenSeq:
    '''
    Sequence number generator (0 - 255 range).
    '''

    def __init__(self):
        self.seq = 0

    def __next__(self):
        self.seq += 1
        if self.seq > 0xff:
            self.seq = 0
        return self.seq

class TweliteException(Exception):

    def __init__(self, message):
        super().__init__(message)

class MasterNode:
    '''
    This is a packet parser for transmitting/receiving data over TWELITE.
    '''

    def __init__(self, port, baudrate, retry=NUM_RETRY, timeout=TIMEOUT):
        self.port  = port
        self.baudrate = baudrate
        self.genSeq = GenSeq()
        self.seq = 0
        self.cmd = None
        self.retry = retry 
        self.timeout = timeout
        self.ser = serial.Serial(self.port, self.baudrate, timeout=timeout)
        
    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.ser.close()

    def close(self):
        self.ser.close()
        
    # Transmit data        
    def _tx(self, dst, cmd):
        self.cmd = cmd
        seq = next(self.genSeq)
        data = [dst, BYTE, seq, RESPONSE_MSG_DISABLED,
                ACK_ENABLED, RESEND, self.retry, TERMINATOR, cmd]
        ck = data[0]
        for c in data[1:]:
            ck = ck ^ c  # XOR for calculating checksum
        len_ = len(data)
        cmd_twelite = bytes([0xA5, 0x5A, 0x80, len_, *data, ck])
        #print([0xA5, 0x5A, 0x80, len_, *data, ck])
        self.ser.write(cmd_twelite)
        self.ser.flush()

    # Receive data
    def _rx(self):
        d = self.ser.read(5)  # 0xA5 0x5A 0x80 <len> <src>
        print(d)
        if len(d) == 0:
            data, seq, lqi = d, 0, 0  # d is b'' in this case
            raise TweliteException('read timeout: {:.1f} sec passed'.format(self.timeout))
        else:
            #print(d)
            len_ =  b2ui(d, 3)
            src = b2ui(d, 4)
            d = self.ser.read(13)  # 0xA0 seq <4bytes> <4bytes> <LQI> 0x00 <len>
            #print(d)
            seq = b2ui(d, 1)
            lqi = b2ui(d, 10)
            len_ = b2ui(d, 12)
            #print('src: {}, len: {}, lqi: {}'.format(str(src), str(len_), str(lqi)))

            data = self.ser.read(len_)  # <data[]>
            #print(data)
                    
            d = self.ser.read(2)  # Checksum, EOT
            #ck = b2i(d,0)
            #print(ck)
        
        return [data, src, seq, lqi]
        
    # Write data
    def write(self, dst, cmd):
        self._tx(dst, cmd)

    def read(self, quality_data=False):
        resp = self._rx()
        if quality_data:
            return resp
        else:
            return resp[0]

    # Read data: tx then rx
    def fetch(self, dst=None, cmd=None, quality_data=False):
        if dst and cmd:
            self._tx(dst, cmd)
        resp = self._rx()
        if quality_data:
            return resp
        else:
            return resp[0]

