#include <SoftwareSerial.h>

SoftwareSerial mySerial(D5, D6);  // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(57600);  // baud rate of the fingerprint sensor
  delay(1000);
}

void loop() {
  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.print(c);
  }

  if (Serial.available()) {
    char c = Serial.read();
    mySerial.write(c);
  }
}
