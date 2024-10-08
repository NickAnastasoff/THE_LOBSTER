/* 
This modified code logs pH and EC readings to a micro SD card with timestamps based on elapsed time.
It removes the RTC module dependencies.
*/

#include <Ezo_i2c.h>        // Include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>           // Include Arduino's I2C library
#include <sequencer2.h>     // Imports a 2-function sequencer
#include <Ezo_i2c_util.h>   // Brings in common print statements
#include <SD.h>             // Include the SD library
#include <SPI.h>            // Include the SPI library

Ezo_board PH = Ezo_board(99, "PH");   // Create a PH circuit object with address 99 and name "PH"
Ezo_board EC = Ezo_board(100, "EC");  // Create an EC circuit object with address 100 and name "EC"

void step1();  // Forward declarations of functions to use them in the sequencer before defining them
void step2();

Sequencer2 Seq(&step1, 1000, &step2, 0);  // Calls the steps in sequence with time in between them

int PH_led = 8;   // Define pin for pH LED
int EC_led = 9;   // Define pin for EC LED

const int chipSelect = 10;  // SD card CS pin

void setup() {
  pinMode(PH_led, OUTPUT);   // Set pin of pH LED as output
  pinMode(EC_led, OUTPUT);   // Set pin for EC LED as output

  Serial.begin(9600);        // Start serial communication to the computer

  // Initialize I2C communication
  Wire.begin();

  // Initialize the SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized.");

  Seq.reset();  // Initialize the sequencer
}

void loop() {
  Seq.run();    // Run the sequencer to do the polling
}

void step1() {
  // Send a read command. We use this command instead of PH.send_cmd("R");
  // to let the library know to parse the reading
  PH.send_read_cmd();
  EC.send_read_cmd();
}

void step2() {
  // Get the elapsed time since the Arduino started in milliseconds
  unsigned long elapsedMillis = millis();

  // Convert milliseconds to seconds, minutes, hours, etc., if needed
  unsigned long totalSeconds = elapsedMillis / 1000;
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;

  // Open the file on the SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // Check if the file is available
  if (dataFile) {
    // Write timestamp to the SD card
    dataFile.print("Time Elapsed: ");
    dataFile.print(hours);
    dataFile.print("h:");
    dataFile.print(minutes);
    dataFile.print("m:");
    dataFile.print(seconds);
    dataFile.print("s, ");

    // Get and log pH reading
    receive_and_print_reading(PH);  // Get the reading from the PH circuit
    float ph_value = PH.get_last_received_reading();
    dataFile.print("pH: ");
    dataFile.print(ph_value);
    Serial.print("pH: ");
    Serial.print(ph_value);

    // LED control for pH
    if (ph_value > 10) {  // Test condition against pH reading
      digitalWrite(PH_led, HIGH);  // If condition true, LED on
    } else {
      digitalWrite(PH_led, LOW);   // If condition false, LED off
    }
    Serial.print("  ");

    dataFile.print(", ");

    // Get and log EC reading
    receive_and_print_reading(EC);  // Get the reading from the EC circuit
    float ec_value = EC.get_last_received_reading();
    dataFile.print("EC: ");
    dataFile.println(ec_value);
    Serial.print("EC: ");
    Serial.println(ec_value);

    // LED control for EC
    if (ec_value > 500.00) {  // Test condition against EC reading
      digitalWrite(EC_led, HIGH);  // If condition true, LED on
    } else {
      digitalWrite(EC_led, LOW);   // If condition false, LED off
    }

    // Close the file
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.txt");
  }
}
