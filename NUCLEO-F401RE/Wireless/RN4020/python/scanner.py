from bluepy import btle

scanner = btle.Scanner(0)
devices = scanner.scan(3.0)

for device in devices:

  print(device.addr)
  print(device.addrType)
  print(device.rssi)

  for (adTypeCode, description, valueText) in device.getScanData():
    print(adTypeCode, description, valueText)
