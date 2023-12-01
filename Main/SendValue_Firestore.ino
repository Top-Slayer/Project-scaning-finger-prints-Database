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

void UpdateFirebase(int id) {
  content.set("fields/ID/integerValue", id);
  content.set("fields/LoginDate/stringValue", UpdateDate());
  content.set("fields/LogoutDate/stringValue", UpdateDate());
  content.set("fields/Status/booleanValue", statusDate);

  if (onceStatus) {
    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), "")) {
      Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), (statusDate) ? "ID, Status, LoginDate" : "ID, Status, LogoutDate");
      onceStatus = false;
      Serial.println("Good 200 status");
    }
    // Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else {
      Serial.println("Bad 400 status");
    }
  } else {
    Serial.println(fbdo.errorReason());
  }

  show_dateStatus[id - 1] = statusDate;
  statusDate = !statusDate;
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

void GetValueFireBase() {
  String path[6];
  path[0] = "FingerPrint/fingerApperance_1";
  path[1] = "FingerPrint/fingerApperance_2";
  path[2] = "FingerPrint/fingerApperance_3";
  path[3] = "FingerPrint/fingerApperance_4";
  path[4] = "FingerPrint/fingerApperance_5";
  path[5] = "FingerPrint/fingerApperance_6";

  for (int i = 0; i < 6; i++) {
    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), "")) {
      content.set("fields/Status/booleanValue", statusDate);
      Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", path[i].c_str(), true);
      show_dateStatus[i] = content.get(result, "fields/Status/booleanValue");
    }
  }

  for (int i = 0; i < 6; i++) {
    Serial.print("ID-Room ");
    Serial.print(i + 1);
    Serial.print(" | Path-document: ");
    Serial.print(path[i]);
    Serial.print(" | Date-status: ");
    Serial.println(show_dateStatus[i]);
  }
}
