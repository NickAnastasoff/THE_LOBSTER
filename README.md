# THE_LOBSTER
# TODO List

THE_LOBSTER collects sensor data from a drifter, sends it via LoRa to a base station, and then uploads it to a website for analysis. Below is the detailed task list for the project.

## 📋 Task List

### 1. Hardware Sensor Integration
- [x] **Get EC (Electrical Conductivity) readings**  
  - [x] Integrate EC sensor.
  - [x] Develop code for reading EC values.

- [x] **Get pH readings**  
  - [x] Integrate pH sensor.
  - [x] Develop code for reading pH values.

- [ ] **Get gyroscope and accelerometer readings for wave height**  
  - [x] Integrate a gyroscope and accelerometer (IMU).
  - [x] Develop code for collecting IMU data.
  - [ ] Implement calculations to determine wave height based on sensor data.
  - [ ] Apply filtering techniques to reduce noise and enhance accuracy of wave height detection.

- [ ] **Get wattage from solar**  
  - [ ] Integrate a solar power
  - [ ] Wire solar to data pins on arduino
  - [x] Develop code for reading wattage values.

- [x] **Get temperature readings**  
  - [x] Integrate temperature sensor.
  - [x] Develop code for reading temperature values.

- [x] **Get flood sensor readings**  
  - [x] Integrate a flood (water level) sensor.
  - [x] Develop code for detecting flood or presence of water.

- [ ] **Get LoRa readings**  
  - [ ] Set up LoRa module for wireless data transmission.
  - [ ] Write code to communicate with the base station.
  - [ ] Optimize power consumption for LoRa module.

### 2. Data Processing
- [x] **Sensor Data Logging**  
  - [x] Develop data logging system on the drifter to collect sensor readings.
  - [x] Add timestamp to each data entry.

### 3. Base Station Communication
- [ ] **Establish LoRa Communication**  
  - [ ] Configure base station to receive data from the drifter.
  - [ ] Develop a protocol for reliable data transfer.
  - [ ] Set up an acknowledgment system to ensure no data is lost. 

- [ ] **Data Storage at Base Station**  
  - [ ] Implement a database or file storage system at the base station.
  - [ ] Add routines to handle data buffering and prevent overflow.

### 4. Website Integration
- [ ] **Use PHP for Data Visualization**  
  - [ ] Set up a PHP website to display data collected from the drifter.
  - [ ] Use charts and gauges to display EC, pH, temperature, and wave height data.
  - [ ] Add features for historical data view and analysis.

- [ ] **Drifter API**  
  - [ ] Write code on the drifter to update settings on card.
  - [ ] Write code on the drifter to send data via lora.
  - [ ] Write code on the drifter to listen for updates via lora.
  - [ ] Write code on the base station to send requests to drifter via lora.

### 5. Power Management
- [ ] **Optimize Power Consumption of Drifter**  
  - [ ] Implement low-power modes for sensors and microcontroller.
  - [ ] Set up solar charging.
  - [ ] Schedule data readings and transmissions to conserve power.
