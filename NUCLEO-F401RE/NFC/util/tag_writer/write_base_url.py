#!/usr/bin/env python3

'''
Write base URL with a local IP address (of wlan0 on RasPi) to
dynamic NFC tag's Area 2.
'''
import serial
import subprocess 
import argparse

PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066AFF575251717867155247-if02'

parser = argparse.ArgumentParser()
parser.add_argument("loc", help="Location")
parser.add_argument("-r", "--ref", help="Reference")
args = parser.parse_args()

local_ip_addr = subprocess.check_output(['hostname', '-I']).decode('utf-8').rstrip(' \n') 

if args.ref:
    base_url = '{}/some_service?loc={}?ref={}\n'.format(local_ip_addr, args.loc, args.ref)
else:
    base_url = '{}/some_service?loc={}\n'.format(local_ip_addr, args.loc)

print(base_url, end='')

with serial.Serial(PORT, 115200, timeout=3) as ser:
    ser.write(base_url.encode('utf-8'))


