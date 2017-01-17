var express = require('express'); // include express.js
var app = express(); // a local instance of it
var http = require('http').Server(app);  //app starts a http server

var dataToSend;

app.use(express.static('public'));

// app.get('/',function(request,response){
//   response.sendFile(__dirname+'/index.html');
// });

function saveData(thisContent) {
  console.log("get "+ thisContent.x+ " "+thisContent.y+" "+thisContent.z);

    dataToSend = {
    x: parseFloat(thisContent.x),
    y: parseFloat(thisContent.y),
    z: parseFloat(thisContent.z)
  };
}

app.get('/data', function(request, response) {
    saveData(request.query);
    response.write("hello");
    response.end();
});

app.get('/current',function(request, response){
response.json(dataToSend);

});

// app.get('/light', function(request, response){
//   response.write(2);
//   response.end();
// });

http.listen(8080, function() {
  console.log('listening on *:8080');  //server listen on port 8080 for connections
});
