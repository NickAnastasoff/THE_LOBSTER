#include <SoftwareSerial.h>
SoftwareSerial softSerial(8, 9);  // RX, TX 
void setup()  {
  softSerial.begin(9600);
  Serial.begin(9600);
} 
void loop()  { 
  if(Serial.available()){
    int outByte = Serial.read();
    softSerial.write(outByte);
  }
  if(softSerial.available()){
    int inByte = softSerial.read();
    Serial.write(inByte);
  }
}