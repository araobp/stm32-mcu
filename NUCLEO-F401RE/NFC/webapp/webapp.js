const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const app = express();
const bodyParser = require('body-parser');
const path = require('path');
const uuidv4 = require('uuid/v4');

// HTTPS default port number
const PORT = 10443 

// Services at the location
const SERVICES = {seoul_22: 'external_links',
                  'yokohama_osanbashi': 'external_links'};

const EXTERNAL_LINKS = {korea_highway_route50_rest_stop: 'https://www.youtube.com/watch?v=eU-tS92t2vU',
  gangneung_noodles: 'https://www.tripadvisor.jp/Restaurant_Review-g317126-d8882864-Reviews-Brothers_Buckwheat_Noodles-Gangneung_Gangwon_do.html',
  yokohama_osanbashi: 'https://osanbashi.jp/english/'
};

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
});

app.get('/some_service', (req, res) => {
  
  // Query strings
  let loc = req.query.loc;
  let dat = req.query.dat;
  let sno = req.query.sno;
  let ref = req.query.ref;
  let tok = req.query.tok;
  let sts = req.query.sts;

  let params = {
    title: loc,
    loc: loc,
    dat: dat,
    sno: sno,
    ref: ref,
    tok: tok,
    sts: sts,
    assign_token: null,
    expires: null
  };

  // Location as reference 
  if (!ref) {
    ref = loc
  }

  console.log('----------')

  if (loc=='debug') {
    
    //----- Debug 
    console.log('>>> Access to debug page');
    res.render('debug', params); 

  } else if (!tok) {

    //----- Access initiated by the tag indirectly
    console.log('>>> Access initiated by dynamic NFC tag');
    // params.expires = expires(3600 * 24)  // expires after one day
    params.expires = expires(60)  // expires after 60 sec (for debugging) 
    params.assign_token = uuidv4();
    res.render('bootstrap', params); 

  } else {

    //----- Service delivery to the browser -----
    console.log('>>> Access initiated by bootstrap html');
    switch(SERVICES[loc]) {

      case 'external_links':
        console.log('Redirect: ' + EXTERNAL_LINKS[ref]);
        res.redirect(EXTERNAL_LINKS[ref]);
        break;

      default:
        switch(sts) {
          case 'new':
            console.log('Access from a new user');
            break;
          case 'entry':
            console.log('Access from a entering user');
            break;
          case 'tracking':
            console.log('Access from a user being tracked');
            break;
        }
        res.render('menu', params); 
        break;
      }
  }
  console.log(params)
  console.log('')
});

// Directory of static HTML files
app.use(express.static(__dirname + '/videoplayer'));

