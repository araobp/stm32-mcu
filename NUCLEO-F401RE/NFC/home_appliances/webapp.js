const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const bodyParser = require('body-parser');
const path = require('path');
const SerialPort = require('serialport');
const os = require('os');

// Serial port to the dynamic NFC tag
const TAG_PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0673FF505250827867103222-if02';
const TAG_REF = ['korea_highway_route50_rest_stop',
                 'gangneung_noodles'];
const tag_port = new SerialPort(TAG_PORT, { baudRate: 115200 });

// RasPi's wlan0 IP address
const local_ip_addr = os.networkInterfaces().wlan0[0].address;
//console.log(local_ip_addr);
const url_base = local_ip_addr + '?loc=seoul_22&ref=';

function writeNextUrl(trn) {
  let url = url_base + TAG[trn] + '\n';
  console.log(url);
  port.write(url, err => {
    if (err) {
      console.log(err.message);
    }
  });
}

// HTTPS default port number
const PORT = 10444

// Express server
const app = express();
app.use(bodyParser.urlencoded({extended: true, limit: '200mb'}));
app.use(bodyParser.json({limit: '200mb'}));

// Directory of static HTML files
app.use(express.static(__dirname));

// EJS template engine setup
app.set('view engine', 'ejs');

// Create HTTPS server for express and ws
let server = https.createServer({
  key: fs.readFileSync(path.join(__dirname + '/server.key')),
  cert: fs.readFileSync(path.join(__dirname + '/server.cert'))
}, app);

app.post('/signage', (req, res) => {
  let trn = req.query.trn;
  console.log(trn);
  writeNextUrl(trn);
  res.send('OK');
});

server.listen(PORT);
