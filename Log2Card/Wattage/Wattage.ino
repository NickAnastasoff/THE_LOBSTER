// Pin configuration
const int batteryPin = A0;  // BAT pin connected to A0
const float referenceVoltage = 5.0; // If using a 5V Arduino, adjust if necessary
const float maxADC = 1023.0;  // Maximum value for a 10-bit ADC

void setup() {
  // Initialize serial communication to view voltage output
  Serial.begin(9600);
}

void loop() {
  // Read the analog input from the BAT pin
  int sensorValue = analogRead(batteryPin);
  
  // Convert the ADC reading to voltage
  float batteryVoltage = (sensorValue / maxADC) * referenceVoltage;

  // Since the charger may have resistive dividers or variations, consider using a calibration factor if necessary.
  // For now, we'll assume a direct reading.
  
  // Output the battery voltage to the Serial Monitor
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);  // Print with 2 decimal places
  Serial.println(" V");
  
  // Delay for a second before the next reading
  delay(1000);
}