/*
Express-based webSocket Server
context: node.js
*/

// Include libraries:
var express = require('express');           // the express library
var http = require("http");                 // the http library
var WebSocketServer = require('ws').Server; // the ws library's Server class

var server = express();                     // the express server
var httpServer = http.createServer(server); // an http server
var wss = new WebSocketServer({ server: httpServer }); // a websocket server

// serve static files from /public:
server.use('/',express.static('public'));


// define the webSocket connection callback function:
function connectClient(newClient) {
  // when a webSocket message comes in from this client:
  function readMessage(data) {
    var result = JSON.parse(data);                // it'll be JSON, so parse it
    if (result.hasOwnProperty('clientName')) {    // if there's a clientName property,
      newClient.clientName = result.clientName;   // use it to name the client
    }
    if (result.hasOwnProperty('button')) {          // or a position property
      broadcast(newClient, result);               // broadcast it for other clients
    }
    if(result.hasOwnProperty('brightness')){
      broadcast(newClient, result);
    }
    console.log(result);                          // print the message
  }

  // if there's a webSocket error:
  function readError(error){
    console.log(error);
  }

  // when a client disconnects from a webSocket:
  function disconnect() {
    console.log('Client disconnected');
  }

  // set up event listeners:
  newClient.on('message', readMessage);
  newClient.on('error', readError);
  newClient.on('close', disconnect);

  // when a new client connects, send the greeting message:
  var greeting = {"client": wss.clients.length};
  newClient.send(JSON.stringify(greeting));
}

// broadcast data to connected webSocket clients:
function broadcast(thisClient, data) {
  for (client in wss.clients) {
    if (wss.clients[client] != thisClient) {
      wss.clients[client].send(JSON.stringify(data));
    }
  }
}

// start the servers:
httpServer.listen(8080, function(){
  console.log('listening on *:8080');  //server listen on port 8080 for connections
});                // listen for http connections
wss.on('connection', connectClient);    // listen for webSocket messages
