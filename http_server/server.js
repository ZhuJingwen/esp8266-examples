var express = require('express'); // include express.js
var server = express(); // a local instance of it
var httpServer = require('http').Server(server);  //app starts a http server

var dataToSend;

server.use(express.static('public'));

function saveData(thisContent) {
  console.log("get "+ thisContent.x+ " "+thisContent.y+" "+thisContent.z);

    dataToSend = {
    x: parseFloat(thisContent.x),
    y: parseFloat(thisContent.y),
    z: parseFloat(thisContent.z)
  };
}

server.get('/data', function(request, response) {
    saveData(request.query);
    response.write("hello");
    response.end();
});

server.get('/led',function(request,response){
  console.log('get led');
  response.write("I got your get request!");
  response.end();
});

httpServer.listen(8080, function() {
  console.log('listening on *:8080');  //server listen on port 8080 for connections
});
