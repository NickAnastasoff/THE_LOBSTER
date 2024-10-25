//d2UgZWF0IHRoZSByYWRpdW0gc28gdGhlIGNsb2NrcyBkb250IGhhdmUgdG8=
#include <SparkFun_RV8803.h>

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

const int batteryPin = A0;  // BAT pin connected to A0
const float referenceVoltage = 5.0; // 5V Arduino
const float maxADC = 1023.0;  // max value for a 10-bit


Ezo_board PH = Ezo_board(99, "PH");       // pH, address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      // Conductivity, address is 100 and name is "EC"
Ezo_board TM = Ezo_board(102, "TM");      // water temp, address is 102 and name is "TM"
DigitalConditionSensor floodSensor("FloodSensor", 7); //create a flood detection sensor object on pin 9
DHT dht(6, DHT22);   //create a dht circut object on pin 8

RV8803 rtc; // Create an instance of the RV8803 RTC
void step1();
void step2();
Sequencer2 Seq(&step1, 1000, &step2, 0);

void setup()  {
  Wire.begin(); 
  softSerial.begin(9600);
  Serial.begin(9600);
  Seq.reset(); 
  dht.begin();

  rtc.begin();
  rtc.setToCompilerTime();
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
  rtc.updateTime(); 
  
}

void step2(){
  receive_and_print_reading(PH);
  receive_and_print_reading(TM);
  receive_and_print_reading(EC);
  Serial.println("");
  
  // read the input from the BAT pin
  int sensorValue = analogRead(batteryPin);
  // convert the ADC reading to voltage
  float batteryVoltage = (sensorValue / maxADC) * referenceVoltage;

  String floodDetection = floodSensor.get_last_received_reading() ? "1" : "0";

  float temp_hum_val[2] = {0};
  dht.readTempAndHumidity(temp_hum_val);

  String currentDate = rtc.stringDateUSA();   // Get the current date in mm/dd/yyyy format
  String currentTime = rtc.stringTime();    // Get the current time in hh:mm:ss format
  String dateTime = currentDate +" " + currentTime; // combine

  // make csv line from all sensor data
  String csvLine = dateTime + "," 
  + String(TM.get_last_received_reading()) +","
  + String(PH.get_last_received_reading()) +","
  + String(EC.get_last_received_reading()) +","
   + floodDetection + ","
   + String(batteryVoltage) +","
   + String(temp_hum_val[1]) +","
   + String(temp_hum_val[0]);
  transmit(csvLine); // send data over serial to esp32 to transmit
  Serial.println(csvLine);
}

void loop()  {  
Seq.run();
delay(3000);
}

