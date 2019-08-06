var app = require('express')();
var server = require('http').Server(app);
var WebSocket = require('ws');

var wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', function connection(ws) {
    console.log('server: receive connection.');
    
    ws.on('message', function incoming(message) {
        console.log('server: received %s', message);
        ws.send('server: reply');
    });

    ws.send('current location');

});

app.get('/', function (req, res) {
  res.sendfile(__dirname + '/ws_client.html');
});

app.listen(8080);