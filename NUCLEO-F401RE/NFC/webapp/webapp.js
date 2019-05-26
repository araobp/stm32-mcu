const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const app = express();
const bodyParser = require('body-parser');
const path = require('path');

// HTTPS default port number
const PORT = 443 

// Services
const SERVICES = {seoul_22: 'restaurants'};
const RESTAURANTS = {korea_highway_route50_rest_stop: 'https://www.youtube.com/watch?v=eU-tS92t2vU',
  gangneung_noodles: 'https://www.tripadvisor.jp/Restaurant_Review-g317126-d8882864-Reviews-Brothers_Buckwheat_Noodles-Gangneung_Gangwon_do.html'};

app.use(bodyParser.urlencoded({extended: true, limit: '200mb'}));
app.use(bodyParser.json({limit: '200mb'}));

// EJS template engine setup
app.set('view engine', 'ejs');

// Directory of EJS templates
app.set('views', path.join(__dirname + '/views')); 

function expires(time) {
  let date = new Date();
  date.setTime(date.getTime()+time*1000);
  let expires = date.toGMTString();
  return expires;
}

// Create HTTPS server
https.createServer({
  key: fs.readFileSync(path.join(__dirname + '/server.key')),
  cert: fs.readFileSync(path.join(__dirname + '/server.cert'))
}, app)
.listen(PORT, () => {
  console.log('HTTPS web server started for web app with dynamic NFC tag');
});

app.get('/some_service', (req, res) => {
  
  let loc = req.query.loc;
  let dat = req.query.dat;
  let sno = req.query.sno;
  let ref = req.query.ref;
  let tok = req.query.tok;

  console.log('---');
  if (sno) {
    console.log('sno: ' + sno);
  }
  if (loc) {
    console.log('loc: ' + loc);
  }
  if (ref) {
    console.log('ref: ' + ref);
  }
  if (tok) {
    console.log('tok: ' + tok);
  }
  if (dat) {
    console.log('dat: ');
    dat.split(',').forEach(it => {
      console.log('- ' + it);
    });
  }

  params = {
    message: 'Welcome!',
    loc: loc,
    dat: dat,
    sno: sno,
    ref: ref,
    expires: expires(3600 * 24)  // expires after one day
  }


  if (loc in SERVICES) {

    switch(SERVICES[loc]) {
      case 'restaurants':
        console.log('Redirect: ' + RESTAURANTS[ref]);
        res.redirect(RESTAURANTS[ref]);
        break;
      defafult:
        break;
    }

  } else {
    res.render('index', params); 
  }
});
