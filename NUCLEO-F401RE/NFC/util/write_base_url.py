#!/usr/bin/env python3

'''
Write base URL with a local IP address (of wlan0 on RasPi) to
dynamic NFC tag's Area 2.
'''

import serial
import subprocess 

PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066AFF575251717867155247-if02'
LOCATION_ID = 'yokohama,osanbashi'

local_ip_addr = subprocess.check_output(['hostname', '-I']).decode('utf-8').rstrip(' \n') 
base_url = '{}/some_service?loc={}\n'.format(local_ip_addr, LOCATION_ID)
print(base_url, end='')

with serial.Serial(PORT, 115200, timeout=3) as ser:
    ser.write(base_url.encode('utf-8'))


