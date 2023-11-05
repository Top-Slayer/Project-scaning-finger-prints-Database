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
void SetupFingerPrint() {
  ESP.wdtDisable();
  myserial.begin(57600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  delay(100);

  if (startFP()) Serial.println("Handshake OK");
  else Serial.println("Handshake Failed!");
}

// ----- Processing function -----

bool CheckingFingerPrint() {
  int p = 0;
  Serial.println("Touch Fingerprint Sensor...");
  while (p == 0) {
    p = readFinger();
    ESP.wdtFeed();
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
    return true; 
  } else if (p == 0) {
    Serial.println("Fingerprint match failed!");
    Serial.println("************************");
    buzzerError();
  }
  return false;  
  // delay(3000);
}