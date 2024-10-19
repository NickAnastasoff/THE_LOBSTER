#include <SoftwareSerial.h>
SoftwareSerial softSerial(8, 9);  // RX, TX 
void setup()  {
  softSerial.begin(9600);
} 
void loop()  { 
  softSerial.write("W"); 
}