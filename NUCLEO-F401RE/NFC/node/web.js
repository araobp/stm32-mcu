const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const app = express();
const bodyParser = require('body-parser');
const path = require('path');

app.use(bodyParser.urlencoded({extended: true, limit: '200mb'}));
app.use(bodyParser.json({limit: '200mb'}));

// Directory of static HTML files
app.use(express.static('./www'));

// EJS template engine setup
app.set('view engine', 'ejs');

app.listen(80, () => {
  console.log('HTTP web server started');
});
