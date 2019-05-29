#
# Gateway implementation for BLE peripherals based on bluepy.
#
# This gateway relays ST25's GPO events to MQTT server.
#

from bluepy import btle
import paho.mqtt.client as mqtt
import argparse
import time
import math
import os
import sys
import traceback

def restart():
    time.sleep(10)
    print("Restarting BLE gateway...")
    python = sys.executable
    os.execl(python, python, *sys.argv)

class Delegate(btle.DefaultDelegate):
    '''
    Delegate to receive notifications from BLE peripheral.
    '''

    def __init__(self, device_name, client):
        btle.DefaultDelegate.__init__(self)
        self.device_name = device_name
        self.mqtt_client = client
        self.mqtt_topic = device_name

    def handleNotification(self, cHandle, data):
        data = data.decode('utf-8')
        print("ST25 GPO event: {}".format(data))
        self.mqtt_client.publish(self.mqtt_topic, data)

class Interface():

    def __init__(self, conn, tx_chara_uuid, rx_chara_uuid):
        self.conn = conn
        self.tx_chara_uuid = tx_chara_uuid
        self.rx_chara_uuid = rx_chara_uuid
        self.write_char = self.conn.getCharacteristics(uuid=rx_chara_uuid)[0]

    def enable_notify(self):
        '''
        Enable notifications on the target characteristic.
        '''
        setup_data = b"\x01\x00"
        notify = self.conn.getCharacteristics(uuid=self.tx_chara_uuid)[0]
        notify_handle = notify.getHandle() + 1
        # In case of RN4020, write fails sometimes. Repeat write three times.
        for i in range(3):
            self.conn.writeCharacteristic(notify_handle, setup_data, withResponse=True)
            time.sleep(1)

if __name__ == '__main__':

    print("Starting BLE gateway...")

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--interface",
                                help="BLE interface number (hcix)", type=int, default=0)
    parser.add_argument("-d", "--device_name", help="BLE device name")
    parser.add_argument("-s", "--service_uuid", help="Service UUID")
    parser.add_argument("-t", "--tx_characteristic_uuid", help="TX characteristic UUID")
    parser.add_argument("-r", "--rx_characteristic_uuid", help="RX characteristic UUID")
    args = parser.parse_args()

    # Scan BLE peripherals
    scanner = btle.Scanner(args.interface)
    devices = scanner.scan(3.0)

    # BLE peripherals
    for device in devices:

        mac_address = None

        # Search for the device name
        for (adTypeCode, description, valueText) in device.getScanData():
            print(valueText)
            if valueText == args.device_name:
                #print(device.addr)
                print(device.addrType)
                #print(device.rssi)
                print(adTypeCode, description, valueText)
                mac_address = device.addr

        # Connect to the BLE peripheral 
        if mac_address: # If found
            peripheral = btle.Peripheral()
            if device.addrType == 'random':
                peripheral.connect(mac_address, btle.ADDR_TYPE_RANDOM) 
            else:
                peripheral.connect(mac_address) 

            desc_list = peripheral.getDescriptors()
            print('--- descriptors ---')
            for desc in desc_list:
                print(desc.uuid, desc.handle, str(desc))

            # List up characterstics supported by the peripheral
            for service in peripheral.getServices():
                if service.uuid == args.service_uuid:
                    print('--- services and characteristics ---')
                    for characteristic in service.getCharacteristics():
                        print('uuid: {}'.format(characteristic.uuid))
                        print('handle: {}'.format(characteristic.getHandle()))
                        print('property string: {}'.format(characteristic.propertiesToString()))
                        print()

            # Interface instance
            interface = Interface(peripheral, args.tx_characteristic_uuid,
                    args.rx_characteristic_uuid)
            interface.enable_notify()  # Enable BLE notify

            # MQTT client setup 
            client = mqtt.Client("ble-router-interface{}".format(args.device_name))
            client.connect("localhost")
            client.loop_start()  # MQTT client starts another thread

            # BLE notification callback function registration 
            peripheral.withDelegate(Delegate(args.device_name,
                                                client))

            # Inifinite loop for waiting notifications from BLE device
            while True:
                try:
                    if peripheral.waitForNotifications(1.0):
                        continue
                except: 
                    traceback.print_exc()
                    restart()

    # Device not found
    restart()

