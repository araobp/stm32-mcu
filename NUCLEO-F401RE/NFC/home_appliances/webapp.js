const express = require('express');
const ejs = require('ejs');
const fs = require('fs');
const https = require('https');
const app = express();
const bodyParser = require('body-parser');
const path = require('path');
const yargs = require('yargs');

// Command arguments
const args = yargs.argv;

// HTTPS default port number
const PORT = 444

app.use(bodyParser.urlencoded({extended: true, limit: '200mb'}));
app.use(bodyParser.json({limit: '200mb'}));

// Directory of static HTML files
app.use(express.static(__dirname));

// EJS template engine setup
app.set('view engine', 'ejs');

if (args.mode == 'https') {
	// Create HTTPS server
	https.createServer({
		key: fs.readFileSync(path.join(__dirname + '/server.key')),
		cert: fs.readFileSync(path.join(__dirname + '/server.cert'))
	}, app)
	.listen(PORT, () => {
	});
} else {
	app.listen(80, () => {
		console.log('HTTP web server started for digital signage web app');
	});
}

