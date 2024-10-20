// ESP32
#define RXD1 18
#define TXD1 17

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
}

void loop() {
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
  if (Serial.available()){
    int inBytes = Serial.read();
    Serial1.write(inBytes);
  }

}
