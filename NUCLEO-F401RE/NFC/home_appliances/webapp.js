const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const bodyParser = require('body-parser');
const path = require('path');
const signage = require('./signage.js');

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
  signage.writeNextUrl(trn);
  res.send('OK');
});

server.listen(PORT);
