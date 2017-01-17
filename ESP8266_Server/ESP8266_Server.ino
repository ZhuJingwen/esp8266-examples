#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Yami-Yichi-2G";
const char* password = "p0rkbun5";

ESP8266WebServer server(80);

const int ledPin = 0;

void handleRoot() {
  digitalWrite(ledPin, LOW);
  Serial.println("/");
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(ledPin, HIGH);
}

void handleLed(){
   server.send(200, "text/plain", "toggle the led");
   Serial.println("/led");
   digitalWrite(ledPin,!digitalRead(ledPin));
}

void handleNotFound(){
  digitalWrite(ledPin, LOW);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(ledPin, HIGH);
}

void setup(void){
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
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

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
    Serial.println("/inline");
  });

  server.on("/led",handleLed);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
