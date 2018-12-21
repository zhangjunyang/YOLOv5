var app = require('express')();
var server = require('http').Server(app);
var WebSocket = require('ws');

function setup() {
    var ws = new WebSocket.Server({ port: 8080 });
    // Listen for the connection open event then call the sendMessage function
    ws.onopen = function(e) {
        log("Connected");
        sendMessage("这是发送的数据")
    }
    // Listen for the close connection event
    ws.onclose = function(e) {
        log("Disconnected: " + e.reason);
    }
    // Listen for connection errors
    ws.onerror = function(e) {
        log("Error ");
    }
    // Listen for new messages arriving at the client
    ws.onmessage = function(e) {
        log("Message received: " + e.data);
    // Close the socket once one message has arrived.
        ws.close();
    }
}
// Send a message on the WebSocket.
function sendMessage(msg){
    ws.send(msg);
    log("Message sent");
}
// Display logging information in the document.
function log(s) {
    var p = document.createElement("p");
    p.style.wordWrap = "break-word";
    p.textContent = s;
    output.appendChild(p);
    // Also log information on the javascript console
    console.log(s);
}

app.get('/', function (req, res) {
  res.sendfile(__dirname + '/test_client.html');
});

app.listen(3000);