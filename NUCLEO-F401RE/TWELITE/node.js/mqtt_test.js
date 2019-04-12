const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://localhost')

client.on('connect', () => {
  client.subscribe('twelite/sensor')
})

client.on('message', (topic, message) => {
  console.log(message.toString('ascii'))
})
