//--- Setup firebase and WiFi ---
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
//--- Time library ---
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Gg ez"
#define WIFI_PASSWORD "nogamenolife"

#define FIREBASE_PROJECT_ID "realtime-apartment"
#define API_KEY "AIzaSyB7OVD3bVd3MJ8__RHY4kBYGiRzo1wiPDk"
#define DATABASE_URL "realtime-apartment-default-rtdb.asia-southeast1.firebasedatabase.app"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);

//--- other variable ---
bool signupOK = false;
bool dateStatus = false;
bool onceStatus = true;
int button = D2;

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT);

  SetupWiFi();
  SetupFirebase();

  timeClient.begin();
}

void loop() {
  // test finger print sensor by button
  if (digitalRead(button) == 1) {
    dateStatus = !dateStatus;
    onceStatus = true;
    delay(20);
  }
  Serial.println(dateStatus);

  UpdateFirebase("Top", dateStatus);
}

// ---- Functions -----
void SetupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("[ Connected ]");
}

void SetupFirebase() {
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void UpdateFirebase(String name, bool dateStatus) {

  FirebaseJson content;
  String documentPath = "FingerPrint/fingerApperance_1";

  content.set("fields/Name/stringValue", name);
  content.set("fields/LoginDate/stringValue", UpdateDate());
  content.set("fields/LogoutDate/stringValue", UpdateDate());
  content.set("fields/Status/booleanValue", dateStatus);

  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), "")) {
    if (onceStatus){
      Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), (dateStatus) ? "Name, Status, LoginDate" : "Name, Status, LogoutDate");
      onceStatus = false;
    }
    // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  } else {
    Serial.println(fbdo.errorReason());
  }
}

String UpdateDate() {
  timeClient.update();
  unsigned long unixTime = timeClient.getEpochTime();

  tmElements_t timeInfo;
  breakTime(unixTime, timeInfo);

  int year = timeInfo.Year + 1970;
  int month = timeInfo.Month;
  int day = timeInfo.Day;

  String formattedDate = String(day) + "/" + String(month) + "/" + String(year) + "-" + String(timeClient.getFormattedTime());

  return formattedDate;
}