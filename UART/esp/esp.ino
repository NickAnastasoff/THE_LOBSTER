// ESP32

#define RXD1 18
#define TXD1 16

void setup() {
  // make both serials (computer and arduino)
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
}

void loop() {
  // Check if a new command is received from Serial (computer)
  if (Serial.available()) {
    char command = Serial.read();

    // send a read request
    if (command == 'R') {
      Serial1.write('R');
    }
  }

  // Check if serial is avaliable, and print it
  if (Serial1.available()) {
    char data = Serial1.read();
    Serial.write(data);
  }
}