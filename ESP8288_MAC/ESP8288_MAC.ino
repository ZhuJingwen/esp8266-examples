#include <ESP8266WiFi.h>

uint8_t MAC_array[6];

void setup() {

  Serial.begin(115200);
  Serial.println();

  WiFi.macAddress(MAC_array);

}

void loop() {
  for (int i = 0; i < sizeof(MAC_array); i++) {
    Serial.print(MAC_array[i], HEX);
    if (i < sizeof(MAC_array)-1) {
      Serial.print(":");
    }
  }
  Serial.println();

  delay(2000);
}
