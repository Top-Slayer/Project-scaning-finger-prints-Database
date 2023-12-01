//////////////////////////////////////////////////////////
void clearBuffer() {
  for (int i = 0; i < 25; i++) Buffer[i] = 0xff;
  //for(int i=0;i<10;i++)myserial.read();
}
//////////////////////////////////////////////////////////
int fpStart() {
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

  while (!myserial.available()) {
    if (digitalRead(switchInput)) {
      state = !state;
      Serial.print("Switch to ");
      Serial.println((state) ? "[Registry Mode]" : "[Check Mode]");
      delay(500);
      break;
    }
  }

  ESP.wdtFeed();

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
int convertToCharArray2() {
  int i = 0;
  for (i = 0; i < 13; i++) {
    myserial.write(img2tz2[i]);
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
int showData(int pageid) {
  int i = 0;
  int sum = 0;
  byte pageid1, pageid2;
  byte sum1, sum2;

  pageid1 = (pageid >> 8) & 0x00ff;
  pageid2 = (pageid & 0x00ff);
  load[11] = pageid1;
  load[12] = pageid2;
  for (i = 6; i < 13; i++) {
    sum += load[i];
  }
  sum1 = (sum >> 8) & 0x00ff;
  sum2 = (sum & 0x00ff);
  load[13] = sum1;
  load[14] = sum2;

  for (i = 0; i < 15; i++) {
    myserial.write(load[i]);
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
int createTemplate() {
  int i = 0;
  for (i = 0; i < 12; i++) {
    myserial.write(regmodel[i]);
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
int createRegistry(int pageid) {
  int i = 0;
  int sum = 0;
  byte pageid1, pageid2;
  byte sum1, sum2;

  pageid1 = (pageid >> 8) & 0x00ff;
  pageid2 = (pageid & 0x00ff);
  store[11] = pageid1;
  store[12] = pageid2;
  for (i = 6; i < 13; i++) {
    sum += store[i];
  }
  sum1 = (sum >> 8) & 0x00ff;
  sum2 = (sum & 0x00ff);
  store[13] = sum1;
  store[14] = sum2;

  for (i = 0; i < 15; i++) {
    myserial.write(store[i]);
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


////////////////////////////////************************************///////////////////////////

// ----- Processing function -----

void SetupFinger() {
  ESP.wdtDisable();
  delay(100);
  Serial.println();
  Serial.println("Start");

  if (fpStart()) Serial.println("Handshake OK");
  else Serial.println("Can't HANDSHAKE");

  Serial.println("Registered ID numbers:");
  for (int j = 0; j < 150; j++) {
    if (showData(j))
      Serial.println(j);
  }
}
bool CheckFinger() {
  int p = 0;
  Serial.println("Touch Fingerprint Sensor...");
  while (p == 0) {
    p = readFinger();
    if (state == true) { return false; }
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
    return true;
  } else if (p == 0) {
    Serial.println("Fingerprint match failed!");
    Serial.println("************************");
  }

  delay(3000);
  return false;
}
void RegistryFinger() {
  int p = 0;
  Serial.println("Touch Fingerprint Sensor to register your finger...");
  while (p == 0) {
    p = readFinger();
    if (state == false) { return; }
  }
  if (p == 1) Serial.println("Fingerprint Registered");
  else if (p == 2) Serial.println("Please Try again...");

  p = convertToCharArray();
  if (p == 1) Serial.println("Converted to Char Array..");
  else if (p == 0) Serial.println("Can't convert to Char Array..");


  p = 0;
  Serial.println("Touch Fingerprint Sensor to register your finger again...");
  while (p == 0) {
    p = readFinger();
  }
  if (p == 1) Serial.println("Fingerprint OK");
  else if (p == 2) Serial.println("Please Try again...");

  p = convertToCharArray2();
  if (p == 1) Serial.println("Converted to Char Array..");
  else if (p == 0) Serial.println("Can't convert to Char Array..");

  p = createTemplate();
  if (p == 1) Serial.println("Tepmlate created, Enter Registration Number (0-150)");
  else if (p == 0) Serial.println("Can't create template..");

  while (!Serial.available())
    ;
  if (Serial.available()) REGISTRY_ID = Serial.parseInt();
  while (Serial.available()) Serial.read();
  (REGISTRY_ID >= 0 && REGISTRY_ID <= 6) ? p = createRegistry(REGISTRY_ID) : Serial.println("Out range of house's number");
  if (p == 1 && REGISTRY_ID >= 0 && REGISTRY_ID <= 6) {
    Serial.print(" Record created with ID:");
    Serial.println(REGISTRY_ID);
  } else if (p == 0) {
    Serial.println("Failed to create record ..");
  } else {
    Serial.println("Unvalid number");
  }
  delay(3000);
}