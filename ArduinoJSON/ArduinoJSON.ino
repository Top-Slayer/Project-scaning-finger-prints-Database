void setup() {
  Serial.begin(115200);  // Set the baud rate to match the Serial Monitor
}

void loop() {
  // Check if there's input available in the Serial buffer
  char input = Serial.read();  // Read a single character
  delay(1000);

  // Perform an action based on the input
  switch (input) {
    case 'a':
      // Do something when 'a' is typed
      Serial.println("You typed 'a'");
      break;
    case 'b':
      // Do something when 'b' is typed
      Serial.println("You typed 'b'");
      break;
    case 'c':
      // Do something when 'c' is typed
      Serial.println("You typed 'c'");
      break;
  }
}
