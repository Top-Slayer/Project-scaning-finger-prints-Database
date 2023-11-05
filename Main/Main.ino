//--- Setup firebase and WiFi ---
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
//--- Time library ---
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
//--- Finger library ---
#include <SoftwareSerial.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// WiFi set SSID and Password
#define WIFI_SSID "Gg ez"
#define WIFI_PASSWORD "nogamenolife"

// Define cloud firestore
#define FIREBASE_PROJECT_ID "realtime-apartment"
#define API_KEY "AIzaSyB7OVD3bVd3MJ8__RHY4kBYGiRzo1wiPDk"
#define DATABASE_URL "realtime-apartment-default-rtdb.asia-southeast1.firebasedatabase.app"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson content;
FirebaseJsonData result;

String documentPath;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);

//--- variables of fingerPrints ---
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

//--- other variable ---
#define buzzerPin D0
bool signupOK = false;
bool dateStatus;
bool onceStatus = true;

void setup() {
  Serial.begin(115200);
  
  SetupWiFi();
  SetupFingerPrint();
  SetupFirebase();

  timeClient.begin();
}

void loop() {
  if (CheckingFingerPrint() == true){
    onceStatus = true;
    switch(REGISTRYID){
      case 1:
        documentPath = "FingerPrint/fingerApperance_1";
        dateStatus = content.get(result, "fields/Status/booleanValue");
        UpdateFirebase(REGISTRYID, dateStatus);
        break;
      case 2:
        documentPath = "FingerPrint/fingerApperance_2";
        dateStatus = content.get(result, "fields/Status/booleanValue");
        UpdateFirebase(REGISTRYID, dateStatus);
        break;
    }
    dateStatus = !dateStatus;
  }
}
