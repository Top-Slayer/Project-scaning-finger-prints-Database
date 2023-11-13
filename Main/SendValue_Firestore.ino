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

void UpdateFirebase(int id, bool dateStatus) {

  content.set("fields/ID/integerValue", id);
  content.set("fields/LoginDate/stringValue", UpdateDate());
  content.set("fields/LogoutDate/stringValue", UpdateDate());
  content.set("fields/Status/booleanValue", dateStatus);

  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), "")) {
    if (onceStatus) {
      Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), (dateStatus) ? "ID, Status, LoginDate" : "ID, Status, LogoutDate");
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
  delay(100);
  return formattedDate;
}