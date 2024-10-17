// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "Grove_Temperature_And_Humidity_Sensor.h"
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>    //include arduinos i2c library
#include <sequencer2.h> //imports a 2 function sequencer 
#include <Ezo_i2c_util.h> //brings in common print statements
#include "logger.h"
#include "DigitalConditionSensor.h"

Logger logger = Logger(10, "values.csv");
Ezo_board PH = Ezo_board(99, "PH");       //create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board EC = Ezo_board(100, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board TM = Ezo_board(102, "TM");      //create an TM circuit object who's address is 102 and name is "TM"
DigitalConditionSensor floodSensor("FloodSensor", 9); //create a flood detection sensor object on pin 9
DHT dht(8, DHT22);   //create a dht circut object on pin 8

void step1();  //forward declarations of functions to use them in the sequencer before defining them
void step2();

Sequencer2 Seq(&step1, 1000, &step2, 0);  //calls the steps in sequence with time in between them

void setup() {
  Wire.begin();                           //start the I2C
  Serial.begin(9600);                     //start the serial communication to the computer
  Seq.reset();                            //initialize the sequencer
  dht.begin();   // Initialize DHT sensor
}

void loop() {
  Seq.run();                              //run the sequencer to do the polling
}

void step1() {
  //send a read command. we use this command instead of PH.send_cmd("R"); 
  //to let the library know to parse the reading
  PH.send_read_cmd();                      
  EC.send_read_cmd();
  TM.send_read_cmd();
  floodSensor.send_read_cmd();            // Send a read command to the flood detection sensor
}

void step2() {
  receive_and_print_reading(PH);             // Get the reading from the PH circuit
  receive_and_print_reading(EC);             // Get the reading from the EC circuit
  receive_and_print_reading(TM);             // Get the reading from the TM circuit

  // Print readings to serial monitor
  // Serial.println();
  // Serial.println("EC");
  // Serial.println(EC.get_last_received_reading());
  // Serial.println("PH");
  // Serial.println(PH.get_last_received_reading());
  // Serial.println("TM");
  // Serial.println(TM.get_last_received_reading());
  // Serial.print("Flood Sensor: ");
  // Serial.println(floodSensor.get_last_received_reading() ? "HIGH" : "LOW");
  // Serial.println("--------");

  logger.begin();
  logger.add(EC.get_last_received_reading());
  logger.add(PH.get_last_received_reading());
  logger.add(TM.get_last_received_reading());
  logger.add(floodSensor.get_last_received_reading() ? "HIGH" : "LOW");
    float temp_hum_val[2] = {0};
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    logger.add(temp_hum_val[1]);//air Temperature
    logger.add(temp_hum_val[0]);//air Humidity
  } else {
    Serial.println("Failed to get temperature and humidity value.");
  }
  logger.endline();
  logger.close();
}
