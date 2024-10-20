// ESP32
#define RXD1 18
#define TXD1 17

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
}

void serialTransmit(String data) {
  Serial1.print(data);
}

String serialRead() {
  String receivedData = "";
  while (Serial1.available()) {
    char incomingByte = Serial1.read();
    receivedData += incomingByte;
  }
  return receivedData;
}

void loop() {
  // Read from Serial1 and send to Serial
  String received = serialRead();
  if (received.length() > 0) {
    Serial.print(received);
  }

  // Read from Serial and send to Serial1
  if (Serial.available()) {
    String inputString = "";
    while (Serial.available()) {
      char incomingByte = Serial.read();
      inputString += incomingByte;
    }
    serialTransmit(inputString);
  }
}
