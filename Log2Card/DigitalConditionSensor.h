#ifndef DIGITAL_CONDITION_SENSOR_H
#define DIGITAL_CONDITION_SENSOR_H

#include <Arduino.h>

class DigitalConditionSensor {
  private:
    String sensor_name;
    uint8_t pin;

  public:
    // Constructor: takes a name and a pin number
    DigitalConditionSensor(const String& name, uint8_t pin_number) : sensor_name(name), pin(pin_number) {
      pinMode(pin, INPUT);  // Set the pin as an input
    }

    // get the last received reading
    bool get_last_received_reading() const {
      return digitalRead(pin);  // read and return the state of the digital pin
    }

    // get the name of the sensor
    String get_name() const {
      return sensor_name;
    }
};

#endif
