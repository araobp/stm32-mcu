from bluepy import btle
import time

DEVICE_NAME = 'RN079D'

scanner = btle.Scanner(0)
devices = scanner.scan(3.0)

MAC_ADDRESS = None
SERVICE_UUID = '11223344-5566-7788-9900-aabbccddeeff'
CHARACTERISTIC_UUID = '01020304-0506-0708-0900-0a0b0c0d0e0f'

def enable_notify(conn, chara_uuid):
    setup_data = b"\x01\x00"
    notify = conn.getCharacteristics(uuid=chara_uuid)[0]
    notify_handle = notify.getHandle() + 1
    conn.writeCharacteristic(notify_handle, setup_data, withResponse=True)

class EdgeAiDelegate(btle.DefaultDelegate):

    def __init__(self):
        btle.DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        #print('handle: {}'.format(cHandle))
        print(int.from_bytes(data, 'little', signed=False))

for device in devices:
    for (adTypeCode, description, valueText) in device.getScanData():
        if valueText == DEVICE_NAME:
            #print(device.addr)
            #print(device.addrType)
            #print(device.rssi)
            print(adTypeCode, description, valueText)
            MAC_ADDRESS = device.addr

    if MAC_ADDRESS:
        peripheral = btle.Peripheral()
        peripheral.connect(MAC_ADDRESS) 
        peripheral.withDelegate(EdgeAiDelegate())

        for service in peripheral.getServices():
            if service.uuid == SERVICE_UUID:
                print('--- services and characteristics ---')
                for characteristic in service.getCharacteristics():
                    print(characteristic.uuid)
                    print(characteristic.getHandle())
                    print(characteristic.propertiesToString()) 
                    pass

        enable_notify(peripheral, CHARACTERISTIC_UUID)

        while True:
            if peripheral.waitForNotifications(1.0):
                #print("Notification")
                continue
