const SerialPort = require('serialport');
const os = require('os');

const PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0673FF505250827867103222-if02';
const INTERVAL = 10 * 1000;
const TAG = ['korea_highway_route50_rest_stop',
             'gangneung_noodles'];

const port = new SerialPort(PORT, { baudRate: 115200 });

// RasPi's wlan0 IP address
const local_ip_addr = os.networkInterfaces().wlan0[0].address; 
console.log(local_ip_addr);

const url_base = local_ip_addr + '?loc=seoul_22&ref=';
let next = 0;

function writeNextUrl() {
  let url = url_base + TAG[next] + '\n';
  console.log(url);
  next++;
  if (next > 1) {
    next = 0;
  }
  port.write(url, err => {
    if (err) {
      console.log(err.message);
    }
  });
}

setInterval(writeNextUrl, INTERVAL);

