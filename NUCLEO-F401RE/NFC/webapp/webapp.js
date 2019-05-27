const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const app = express();
const bodyParser = require('body-parser');
const path = require('path');
const uuidv4 = require('uuid/v4');

// HTTPS default port number
const PORT = 443 

// Services
const SERVICES = {seoul_22: 'restaurants',
                  'yokohama,osanbashi': 'debug'};
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
  
  // Query strings
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

  let params = {
    message: 'Welcome to the dynamic NFC tag world!',
    loc: loc,
    dat: dat,
    sno: sno,
    ref: ref,
    tok: tok,
    assign_token: null,
    //expires: expires(3600 * 24)  // expires after one day
    expires: expires(5)  // expires after 5 sec (for debugging) 
  };


  //----- Service delivery -----

    switch(SERVICES[loc]) {
      case 'restaurants':
        console.log('Redirect: ' + RESTAURANTS[ref]);
        res.redirect(RESTAURANTS[ref]);
        break;
      case 'debug':
        console.log('Access to debug page');
        res.render('debug', params); 
        break;
      default:
        if (tok) {
          console.log(tok);
          if (tok == '0') {
            console.log('Access to menu page without token');
            params.assign_token = uuidv4();
          } else {
            console.log('Access to menu page with token');
          }
          res.render('menu', params); 
        } else {
          console.log('Access to bootstrap page');
          res.render('bootstrap', params); 
        }
        break;
    }
});
