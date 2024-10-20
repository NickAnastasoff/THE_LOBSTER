#include <SoftwareSerial.h>
#include "Grove_Temperature_And_Humidity_Sensor.h"
#include <Ezo_i2c.h>
#include <Wire.h>
#include <sequencer2.h>
#include <Ezo_i2c_util.h>
#include "logger.h"
#include "DigitalConditionSensor.h"
#include <SparkFun_RV8803.h>

SoftwareSerial softSerial(8, 9);  // RX, TX 

Ezo_board PH = Ezo_board(99, "PH");       // pH, address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      // Conductivity, address is 100 and name is "EC"
Ezo_board TM = Ezo_board(102, "TM");      // water temp, address is 102 and name is "TM"

void step1();
void step2();
Sequencer2 Seq(&step1, 1000, &step2, 0);

void setup()  {
  Wire.begin(); 
  softSerial.begin(9600);
  Serial.begin(9600);
  Seq.reset(); 
  Serial.println("3");
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

void step1() {
  PH.send_read_cmd();                      
  EC.send_read_cmd();
  TM.send_read_cmd();
  // floodSensor.send_read_cmd();    
  // rtc.updateTime();  
}

void step2(){
  receive_and_print_reading(PH);
  receive_and_print_reading(TM);
  receive_and_print_reading(EC);
  
  transmit("EC " + String(EC.get_last_received_reading()));                           // Conductivity
  transmit("pH " + String(PH.get_last_received_reading()));                           // pH
  transmit("TM " + String(TM.get_last_received_reading()));
}

void loop()  {  
Seq.run(); 
}

