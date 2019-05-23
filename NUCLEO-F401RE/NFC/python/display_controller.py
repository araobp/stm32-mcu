#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time
import serial
import subprocess

PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_066AFF575251717867155247-if02'

INTERVAL = 10  # in seconds

TAG_DATA = {'1': ['seoul_22', 'korea_highway_route50_rest_stop'],
        '2': ['seoul_22', 'gangneung_noodles']}

LOCAL_IP_ADDR = subprocess.check_output(['hostname', '-I']).decode('utf-8').rstrip(' \n')

URL_FORMAT = LOCAL_IP_ADDR + "/some_service?loc={}&ref={}\n"

client = mqtt.Client('display_controller')
client.connect('localhost')

with serial.Serial(PORT, 115200, timeout=3) as ser:

    while True:

        url = URL_FORMAT.format(*TAG_DATA['1']) 
        print(url, end='')
        ser.write(url.encode('utf-8'))
        client.publish('display', '1')
        time.sleep(INTERVAL)

        url = URL_FORMAT.format(*TAG_DATA['2']) 
        print(url, end='')
        ser.write(url.encode('utf-8'))
        client.publish('display', '2')
        time.sleep(INTERVAL)


