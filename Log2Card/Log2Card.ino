/*This code was written for the Instructable "MAKE YOUR OWN PH AND SALINITY MONITORING SYSTEM WITH LED INDICATORS" (Link: https://www.instructables.com/id/MAKE-YOUR-OWN-PH-EC-MONITOR-WITH-LED-INDICATORS/)
and hackster.io "Make Your Own pH and Salinity Monitoring System" (Link: https://www.hackster.io/atlas-scientific/make-your-own-ph-and-salinity-monitoring-system-fb14c1)
Testing was done using an Arduino UNO.
The code allows you to monitor in real-time, pH and EC. You can modify it to observe other parameters such as DO, temperature and ORP.  It works 
with Atlas Scientific's EZO circuits. The sensors must be calibrated and switched to I2C mode before using this code as it does not have the capability 
of allowing the user to send commands to the circuits. The readings of the sensors are displayed on the Arduino serial monitor. 
There are two LEDs which functions as a warning system. They are turned on when the readings go out of the defined limits.
These LEDs offer a simple demonstration of how you can utilize sensor readings to trigger other hardware.
Once you have uploaded the code to your Arduino, open the serial monitor, set the baud rate to 9600 and append "Carriage return"*/

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
DigitalConditionSensor floodSensor("FloodSensor", 9); //create a flood detection sensor object on pin 13

void step1();  //forward declarations of functions to use them in the sequencer before defining them
void step2();

Sequencer2 Seq(&step1, 1000, &step2, 0);  //calls the steps in sequence with time in between them

void setup() {
  Wire.begin();                           //start the I2C
  Serial.begin(9600);                     //start the serial communication to the computer
  Seq.reset();                            //initialize the sequencer
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
  Serial.println();
  Serial.println("EC");
  Serial.println(EC.get_last_received_reading());
  Serial.println("PH");
  Serial.println(PH.get_last_received_reading());
  Serial.println("TM");
  Serial.println(TM.get_last_received_reading());
  Serial.print("Flood Sensor: ");
  Serial.println(floodSensor.get_last_received_reading() ? "HIGH" : "LOW");

  Serial.println("--------");

  // Log readings
  logger.begin();
  logger.add(EC.get_last_received_reading());
  logger.add(PH.get_last_received_reading());
  logger.add(TM.get_last_received_reading());
  logger.add(floodSensor.get_last_received_reading() ? "HIGH" : "LOW");
  logger.endline();
  logger.close();
}
