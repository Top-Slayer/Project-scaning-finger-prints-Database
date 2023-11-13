#include <SoftwareSerial.h>
#define buzzerPin D0

SoftwareSerial myserial(D5, D6);  // RX, TX

const byte handshake[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x17, 0x00, 0x00, 0x1c };
const byte getimg[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05 };
const byte img2tz1[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08 };
const byte img2tz2[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08 };
const byte regmodel[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09 };
const byte store[14] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };
const byte search[17] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0xff, 0x01, 0x0d };
int REGISTRYID, SCORE;



int Buffer[25];

//////////////////////////////////////////////////////////
void clearBuffer() {
  for (int i = 0; i < 25; i++) Buffer[i] = 0xff;
  //for(int i=0;i<10;i++)myserial.read();
}
//////////////////////////////////////////////////////////
int startFP() {
  int i = 0;
  for (i = 0; i < 13; i++) {
    myserial.write(handshake[i]);
  }

  while (!myserial.available())
    ;
  i = 0;
  while (i < 12) {
    if (myserial.available()) {
      Buffer[i] = myserial.read();
      i++;
    }
  }

  if (Buffer[9] == 0) {
    clearBuffer();
    return 1;
  } else {
    clearBuffer();
    return 0;
  }
}
//////////////////////////////////////////////////////////
int readFinger() {
  int i = 0;
  for (i = 0; i < 12; i++) {
    myserial.write(getimg[i]);
  }
  while (!myserial.available())
    ;
  i = 0;
  while (i < 12) {
    if (myserial.available()) {
      Buffer[i] = myserial.read();
      i++;
    }
  }

  if (Buffer[9] == 0) {
    clearBuffer();
    return 1;
  } else if (Buffer[9] == 1 || Buffer[9] == 3) {
    clearBuffer();
    return 2;
  } else {
    clearBuffer();
    return 0;
  }
}
//////////////////////////////////////////////////////////
int convertToCharArray() {
  int i = 0;
  for (i = 0; i < 13; i++) {
    myserial.write(img2tz1[i]);
  }

  while (!myserial.available())
    ;
  i = 0;
  while (i < 12) {
    if (myserial.available()) {
      Buffer[i] = myserial.read();
      i++;
    }
  }

  if (Buffer[9] == 0) {
    clearBuffer();
    return 1;
  } else {
    clearBuffer();
    return 0;
  }
}
//////////////////////////////////////////////////////////
int findFP() {
  int i = 0;
  for (i = 0; i < 17; i++) {
    myserial.write(search[i]);
  }

  while (!myserial.available())
    ;
  i = 0;
  while (i < 16) {
    if (myserial.available()) {
      Buffer[i] = myserial.read();
      i++;
    }
  }

  if (Buffer[9] == 0) {
    REGISTRYID = (Buffer[10] << 8) + (Buffer[11] & 0x00ff);
    SCORE = (Buffer[12] << 8) + (Buffer[13] & 0x00ff);
    clearBuffer();
    return 1;
  } else {
    clearBuffer();
    return 0;
  }
}
//////////////////////////////////////
void buzzerOK() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(30);
    digitalWrite(buzzerPin, LOW);
    delay(30);
  }
}

void buzzerError() {
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
}
//////////////////////////////////////
void setup() {
  ESP.wdtDisable();
  Serial.begin(115200);
  myserial.begin(57600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  delay(100);

  if (startFP()) Serial.println("Handshake OK");
  else Serial.println("Handshake Failed!");
}

void loop() {
  working();
}

void working() {
  int p = 0;
  Serial.println("Touch Fingerprint Sensor...");
  while (p == 0) {
    p = readFinger();
    ESP.wdtFeed();
      Serial.print(myserial);
      Serial.print(" ");
    Serial.println();
  }
  if (p == 1) Serial.println("Finger OK");
  else if (p == 2) Serial.println("Please try again!");

  p = convertToCharArray();
  if (p == 1) Serial.println("Converted to char array..");
  else if (p == 2) Serial.println("Converting to char array failed!");

  p = findFP();
  if (p == 1) {
    Serial.print("Fingerprint Matched, Registry ID:");
    Serial.println(REGISTRYID);
    Serial.print("Matching Score:");
    Serial.println(SCORE);
    buzzerOK();
  } else if (p == 0) {
    Serial.println("Fingerprint match failed!");
    Serial.println("************************");
    buzzerError();
  }

  delay(3000);
}