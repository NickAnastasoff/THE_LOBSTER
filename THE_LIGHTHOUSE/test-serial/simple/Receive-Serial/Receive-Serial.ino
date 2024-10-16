#include <RadioLib.h>

SX1262 radio = new Module(D36, D40, D44, D39, SPI1);

void setup() {
  Serial.begin(9600);

  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(915.0, 250.0, 7, 5, 0x34, 20, 10, 0, false);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[SX1262] Waiting for incoming transmission ... "));

  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));

    // Sending the data in a parsable format
    Serial.print(F("DATA:"));  // Label the data
    Serial.println(str);

    Serial.print(F("RSSI:"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    Serial.print(F("SNR:"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("timeout!"));
  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    Serial.println(F("CRC error!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}
