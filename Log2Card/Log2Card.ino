#include "Grove_Temperature_And_Humidity_Sensor.h"
#include <Ezo_i2c.h>
#include <Wire.h>
#include <sequencer2.h>
#include <Ezo_i2c_util.h>
#include "logger.h"
#include "DigitalConditionSensor.h"
#include <SparkFun_RV8803.h> // Include the SparkFun RV-8803 RTC library

Logger logger = Logger(10, "values.csv");
Ezo_board PH = Ezo_board(99, "PH");       // pH, address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      // Conductivity, address is 100 and name is "EC"
Ezo_board TM = Ezo_board(102, "TM");      // water temp, address is 102 and name is "TM"
DigitalConditionSensor floodSensor("FloodSensor", 9); //create a flood detection sensor object on pin 9
DHT dht(8, DHT22);   //create a dht circut object on pin 8

RV8803 rtc; // Create an instance of the RV8803 RTC

void read();  //forward declarations of functions to use them in the sequencer before defining them
void log();

Sequencer2 Seq(&read, 1000, &log, 0);  //calls the steps in sequence with time in between them

void setup() {
  Wire.begin();                           //start the I2C
  Serial.begin(9600);                     //start the serial communication to the computer
  Seq.reset();                            //initialize the sequencer
  dht.begin();   // Initialize DHT sensor

  // Initialize the RTC
rtc.begin();
  
rtc.setToCompilerTime();

}

void loop() {
  Seq.run();                              //run the sequencer to do the polling
}

void read() {
  PH.send_read_cmd();                      
  EC.send_read_cmd();
  TM.send_read_cmd();
  floodSensor.send_read_cmd();    
     rtc.updateTime();  
}

void log() {
  float temp_hum_val[2] = {0};
  dht.readTempAndHumidity(temp_hum_val);

  logger.begin();

  String currentDate = rtc.stringDateUSA(); // Get the current date in mm/dd/yyyy format
  String currentTime = rtc.stringTime();    // Get the current time in hh:mm:ss format
  String dateTime = currentDate + " " + currentTime;
  logger.add(dateTime);                     // Log the date and time

  // Print the timestamp to serial for debugging
  Serial.print("Timestamp: ");
  Serial.println(dateTime);

  // Log other sensor data
  logger.add(EC.get_last_received_reading());                           // Conductivity
  logger.add(PH.get_last_received_reading());                           // pH
  logger.add(TM.get_last_received_reading());                           // Water temperature
  logger.add(floodSensor.get_last_received_reading() ? "HIGH" : "LOW"); // Flood detection
  logger.add(temp_hum_val[1]);                                          // Air Temperature
  logger.add(temp_hum_val[0]);                                          // Air Humidity
  logger.endline();
  logger.close();
}
