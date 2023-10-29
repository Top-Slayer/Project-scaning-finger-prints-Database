#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>


#define WIFI_SSID "Alisone"
#define WIFI_PASSWORD "56661595"

#define API_KEY "AIzaSyB7OVD3bVd3MJ8__RHY4kBYGiRzo1wiPDk"
#define DATABASE_URL "realtime-apartment.firebaseio.com"
#define USER_EMAIL "hummen55@gmail.com"
#define USER_PASSWORD "noynoyys55"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("[ Connected ]");

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(1024, 1024);

  Firebase.begin(&config, &auth);
}

void loop() {
  if (Firebase.ready() == 1) {
    Firebase.get(fbdo, "/1/finger_prints/ID_1");
    String value = fbdo.stringData();
    Serial.print("Value from Firestore: ");
    Serial.println(value);
  }

  delay(1000);
}
