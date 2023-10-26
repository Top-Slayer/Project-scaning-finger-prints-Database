#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>


// Variables connecting WiFi
// --------------------------------------
const char* ssid = "Gg ez";
const char* password = "nogamenolife";
// --------------------------------------


// Variables of Checking finger prints
// --------------------------------------
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
// --------------------------------------


// Other variables
// --------------------------------------
int LED = D4;
// --------------------------------------

ESP8266WebServer server(80);

void setup() {
  ESP.wdtDisable();
  Serial.begin(115200);
  myserial.begin(57600);
  delay(100);


  // Other setup
  // --------------------------------------
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // --------------------------------------


  // Connecting to WiFi and starting Server
  // --------------------------------------
  Serial.print("\n\nConnecting to Internet ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  displayIP();

  /*-------- server started---------*/
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleOn);
  server.on("/off", HTTP_GET, handleOff);

  server.begin();

  Serial.println("Server started");
  // --------------------------------------

  if (startFP()) Serial.println("Handshake OK");
  else Serial.println("Handshake Failed!");
}

void loop() {
  // server.handleClient();

  char num = Serial.read();
  switch (num) {
    case '0':
      Serial.println("sdsd");
      break;
    case '1':
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
      } else if (p == 0) {
        Serial.println("Fingerprint match failed!");
        Serial.println("************************");
      }

      delay(3000);
      break;
  }
}

// Function
void displayIP() {
  Serial.print("\nLocal IP: ");
  Serial.print(WiFi.localIP());
  Serial.print(" - ");
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("Host name: ");
  Serial.print(WiFi.hostname());
  Serial.print(" - ");
  Serial.print("Hotspot name: ");
  Serial.println(WiFi.SSID());
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP8266 Web Control</h1>";
  html += "<p>Click <a href='/on'>here</a> to turn the LED on.</p>";
  html += "<p>Click <a href='/off'>here</a> to turn the LED off.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(LED, HIGH);  // Turn the LED on
  server.send(200, "text/plain", "LED is on");
}

void handleOff() {
  digitalWrite(LED, LOW);  // Turn the LED off
  server.send(200, "text/plain", "LED is off");
}

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