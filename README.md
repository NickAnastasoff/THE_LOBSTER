# THE_LOBSTER
# TODO List

THE_LOBSTER collects sensor data from a drifter, sends it via LoRa to a base station, and then uploads it to a website for analysis. Below is the detailed task list for the project.

## 📋 Task List

### 1. Hardware Sensor Integration
- [x] **Get EC (Electrical Conductivity) readings**  
  - [x] Integrate EC sensor.
  - [x] Develop code for reading EC values.

- [ ] **Get pH readings**  
  - [x] Integrate pH sensor.
  - [ ] Develop code for reading pH values.

- [ ] **Get gyroscope and accelerometer readings for wave height**  
  - [ ] Integrate a gyroscope and accelerometer (IMU).
  - [ ] Develop code for collecting IMU data.
  - [ ] Implement calculations to determine wave height based on sensor data.
  - [ ] Apply filtering techniques to reduce noise and enhance accuracy of wave height detection.

- [ ] **Get temperature readings**  
  - [ ] Integrate temperature sensor.
  - [ ] Develop code for reading temperature values.

- [ ] **Get flood sensor readings**  
  - [ ] Integrate a flood (water level) sensor.
  - [ ] Develop code for detecting flood or presence of water.

- [ ] **Get LoRa readings**  
  - [ ] Set up LoRa module for wireless data transmission.
  - [ ] Write code to communicate with the base station.
  - [ ] Optimize power consumption for LoRa module.

### 2. Data Processing
- [ ] **Sensor Data Logging**  
  - [ ] Develop data logging system on the drifter to collect sensor readings.
  - [ ] Add timestamp to each data entry.

- [ ] **Data Compression**  
  - [ ] Implement data compression techniques to reduce data size before transmission.
  - [ ] Evaluate lightweight compression algorithms for efficient data handling.

### 3. Base Station Communication
- [ ] **Establish LoRa Communication**  
  - [ ] Configure base station to receive data from the drifter.
  - [ ] Develop a protocol for reliable data transfer.
  - [ ] Set up an acknowledgment system to ensure no data is lost. 

- [ ] **Data Storage at Base Station**  
  - [ ] Implement a database or file storage system at the base station.
  - [ ] Add routines to handle data buffering and prevent overflow.

### 4. Cloud Integration
- [ ] **Use Blynk for Data Visualization**  
  - [ ] Set up a Blynk project to visualize data collected from the drifter.
  - [ ] Use charts and gauges to display EC, pH, temperature, and wave height data.
  - [ ] Add features for historical data view and analysis.

- [ ] **Make MQTT API for Data Transfer**  
  - [ ] Develop an MQTT API for the base station to communicate with Blynk and the website.
  - [ ] Ensure MQTT API supports secure and reliable data upload.
  - [ ] Add functionality for real-time data retrieval and efficient data transfer.

- [ ] **Data Upload System**  
  - [ ] Write code at the base station to periodically upload data to the cloud using MQTT.
  - [ ] Handle network outages with retry mechanisms to ensure data reliability.

### 5. Power Management
- [ ] **Optimize Power Consumption of Drifter**  
  - [ ] Implement low-power modes for sensors and microcontroller.
  - [ ] Set up solar charging.
  - [ ] Schedule data readings and transmissions to conserve power.
