/*
   This example is built based on Tom Igoe's VideoControllerWebSocket example
*/

#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

const char* ssid = "your-ssid";
const char* password = "your-password";

const char serverAddress[] = "192.168.0.5";  // server address
int port = 8080;                              // port number
WiFiClient tcpSocket;                         // server socket

// make a websocket instance
WebSocketClient webSocket = WebSocketClient(tcpSocket, serverAddress, port);

int buttonPin = 16;
int buttonState = 0;
int prevButtonState = 0;

int ledPin = 15;
int brightness = 200;
int ledPinRed = 0; //huzzah on board red LED

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinRed, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!webSocket.connected()) {      // if you're not connected,
    connectToServer();               // connect to server
  }
}

void loop() {

  if (!webSocket.connected()) {      // if you're not connected,
    connectToServer();               // connect to server
  }

  // while websocket is connected, listen for incoming messages:
  if (webSocket.connected()) {
    int msgLength = webSocket.parseMessage();  // get message length
    if (msgLength > 0) {                       // if it's > 0,
      String message = webSocket.readString(); // read it
      Serial.println(message);                 // print it

      //parsing incoming message in JSON format
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(message);
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      int b = root["brightness"]; //parsing brightness value from server
      brightness = b;
    }
  }

  // set the LED with the brightness variable:
  analogWrite(ledPin, brightness);

  // set the on board red LED with the brightness variable:
  if(brightness > 100){
    digitalWrite(ledPinRed,LOW);
  }else{
    digitalWrite(ledPinRed,HIGH);
  }

  //read button states
  buttonState = digitalRead(buttonPin);
  if (buttonState != prevButtonState) {
    if (webSocket.connected()) {
      sendJsonMessage("button", buttonState);
    }
  }
  prevButtonState = buttonState;
}

// This function forms a JSON message to send,
// from a key-value pair:
void sendJsonMessage(String key, int val) {
  webSocket.beginMessage(TYPE_TEXT);   // message type: text
  webSocket.print("{\"clientName\":\"ESP8266\"");
  if (key != "") {          // if there is no key, just send name
    webSocket.print(",\""); // comma, opening quotation mark
    webSocket.print(key);   // key
    webSocket.print("\":"); // closing quotation mark, colon
    webSocket.print(val);   // value
  }
  webSocket.print("}");
  webSocket.endMessage();
}

void connectToServer() {
  Serial.println("attempting to connect");
  boolean error = webSocket.begin();   // attempt to connect
  if (error) {
    Serial.println("failed to connect");
  } else {
    Serial.println("connected");
    sendJsonMessage("", 0);  // send the client name and nothing else
  }
}
