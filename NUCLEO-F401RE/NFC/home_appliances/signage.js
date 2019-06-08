const SerialPort = require('serialport');
const os = require('os');

// Serial port to the dynamic NFC tag for signage
const TAG_PORT = '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0673FF505250827867103222-if02';
const TAG_REF = ['korea_highway_route50_rest_stop',
                 'gangneung_noodles'];
const tag_port = new SerialPort(TAG_PORT, { baudRate: 115200 });

// RasPi's wlan0 IP address
const local_ip_addr = os.networkInterfaces().wlan0[0].address;
//console.log(local_ip_addr);
const url_base = local_ip_addr + ':10443/some_service?loc=seoul_22&ref=';

// Write next URL to the dynamic NFC tag
module.exports.writeNextUrl = function(trn) {
  let url = url_base + TAG_REF[trn] + '\n';
  console.log(url);
  tag_port.write(url, err => {
    if (err) {
      console.log(err.message);
    }
  });
}
