#include <FS.h>
#include <ArduinoJson.h>

const size_t capacity = 200; // Adjust the capacity based on your data size
StaticJsonDocument<capacity> jsonDoc;

void setup() {
    Serial.begin(115200);

    // Initialize SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }

    // Add data to the JSON object
    jsonDoc["sensor"] = "DHT22";
    jsonDoc["temperature"] = 25.5;
    jsonDoc["humidity"] = 60;

    // Open a file for writing
    File jsonFile = SPIFFS.open("/data.json", "r");
    if (!jsonFile) 
        Serial.println("Failed to open file for writing");
        return;
    }

    // Serialize the JSON data and write it to the file
    serializeJson(jsonDoc, jsonFile);

    // Close the file
    jsonFile.close();

    Serial.println("\nJSON data written to file");
}

void loop() {
    // Your main code here
}
