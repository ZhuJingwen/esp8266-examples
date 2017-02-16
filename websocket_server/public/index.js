/*
socket client
context: P5.js
*/

// set global variables:

var socket;                           // the websocket
var lastMessage;                      // last message received from server
var host = document.location.host;    // the server that served this page
var message = {"clientName": "browser"};  // what you'll send to the server
var bSlider;
var prevB = 100;

function setup() {                 // loop the video
  lastMessage = createDiv('');        // create a div for text messages
  lastMessage.position(10, 10);       // position the div

  bSlider = createSlider(0,255,100);
  bSlider.position(10,40);

  socket = new WebSocket('ws://' + host); // connect to server
  console.log(host);
  socket.onopen = sendIntro;          // socket connection listener
  socket.onmessage = readMessage;     // socket message listener
}

function draw(){
  //check slider value
  var b = bSlider.value();
  if(b != prevB){
    var msgToSend = {"brightness":b};
    socket.send(JSON.stringify(msgToSend));
  }
  prevB = b;
}
function sendIntro() {
  // convert the message object to a string and send it:
  socket.send(JSON.stringify(message));
  console.log("sent intro");
}

function readMessage(event) {
  // read  text from server:
  var msg = event.data;           // read data from the onmessage event
  var message = JSON.parse(msg);  // convert incoming message to JSON
  console.log(message);
  if (message.hasOwnProperty('button')) {
    //using button value to change backgroundColor
    if(message.button>0){
      document.body.style.backgroundColor = "grey";
    }else{
      document.body.style.backgroundColor = "white";
    }
  }

  // save the last client message received for printing on the screen:
  lastMessage.html(JSON.stringify(message));
}
