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
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000); // GMT +7 -- Set time zone

//--- variables of fingerPrints ---
byte handshake[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x17, 0x00, 0x00, 0x1c };
byte getimg[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05 };
byte img2tz1[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08 };
byte img2tz2[13] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x02, 0x00, 0x09 };
byte regmodel[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09 };
byte store[15] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };
byte search[17] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0xff, 0x01, 0x0d };
byte load[15] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00 };
int REGISTRY_ID = 0;
int REGISTRYID, SCORE;
SoftwareSerial myserial(D5, D6);  //RX,TX
int Buffer[25];

//--- other variable ---
bool signupOK = false;
bool dateStatus = true;
bool onceStatus = true;

const int switchInput = D2;
bool state = false;

void setup() {
  Serial.begin(115200);
  myserial.begin(57600);
  
  SetupWiFi();
  SetupFirebase();
  SetupFinger();

  pinMode(switchInput, INPUT);

  timeClient.begin();
}

void loop() {
  Serial.println((state) ? "\n>> Registry Mode:" : "\n>> Check Mode:");

  if (state) {
    RegistryFinger();
  } else {
    if (CheckFinger() == true) {
      onceStatus = true;
      switch (REGISTRYID) {
        case 1:
          documentPath = "FingerPrint/fingerApperance_1";
          dateStatus = !dateStatus;
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, dateStatus);
          break;
        case 2:
          documentPath = "FingerPrint/fingerApperance_2";
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, !dateStatus);
          break;
        case 3:
          documentPath = "FingerPrint/fingerApperance_3";
          dateStatus = !dateStatus;
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, dateStatus);
          break;
        case 4:
          documentPath = "FingerPrint/fingerApperance_4";
          dateStatus = !dateStatus;
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, dateStatus);
          break;
        case 5:
          documentPath = "FingerPrint/fingerApperance_5";
          dateStatus = !dateStatus;
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, dateStatus);
          break;
        case 6:
          documentPath = "FingerPrint/fingerApperance_6";
          dateStatus = !dateStatus;
          dateStatus = content.get(result, "fields/Status/booleanValue");
          UpdateFirebase(REGISTRYID, dateStatus);
          break;
      }
    }
  }
}
