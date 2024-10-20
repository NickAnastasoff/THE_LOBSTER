#include <SoftwareSerial.h>
#include <Ezo_i2c.h>
#include <Wire.h>
#include <sequencer2.h>
#include <Ezo_i2c_util.h>

SoftwareSerial softSerial(8, 9);  // RX, TX 

Ezo_board PH = Ezo_board(99, "PH");       // pH, address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      // Conductivity, address is 100 and name is "EC"
Ezo_board TM = Ezo_board(102, "TM");      // water temp, address is 102 and name is "TM"

Sequencer2 Seq(&read, 1000, &step2, 0);

void setup()  {
  Wire.begin(); 
  softSerial.begin(9600);
  Serial.begin(9600);
  Seq.reset(); 
} 

void transmit(String output) {
  softSerial.println(output);
}
void recieve(){
  if(softSerial.available()){
    int inByte = softSerial.read();
    return inByte;
  }
}

void read() {
  PH.send_read_cmd();                      
  EC.send_read_cmd();
  TM.send_read_cmd();
  // floodSensor.send_read_cmd();    
  // rtc.updateTime();  
}

void step2(){
  transmit("EC " + String(EC.get_last_received_reading()));                           // Conductivity
  transmit("pH " + String(PH.get_last_received_reading()));                           // pH
  transmit("TM " + String(TM.get_last_received_reading()));
  Serial.println(PH.get_last_received_reading());
  Serial.println(TM.get_last_received_reading());
  Serial.println(EC.get_last_received_reading());
}
void loop()  {  
Seq.run(); 
}

