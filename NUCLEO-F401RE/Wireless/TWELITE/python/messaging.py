import paho.mqtt.client as mqtt
import time

client = mqtt.Client("twelite-master-node")
client.connect("localhost")

seq = 0

while True:
    client.publish("twelite/sensor", seq)
    seq += 1
    time.sleep(1)

