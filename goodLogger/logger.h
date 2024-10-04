#ifndef LOGGER_H
#define LOGGER_H

#include <SD.h>

class Logger {
public:
    Logger(int chipSelectPin, const char *filename) {
        chipSelect = chipSelectPin;
        fileName = filename;
    }

    void begin() {
        if (!SD.begin(chipSelect)) {
            Serial.println("SD card initialization failed!");
            return;
        }
        file = SD.open(fileName, FILE_WRITE);
        if (!file) {
            Serial.println("Error opening file!");
        }
    }

    // Overloaded add method for String
    void add(const String &value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for const char*
    void add(const char *value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for int
    void add(int value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for float
    void add(float value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for double
    void add(double value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Add support for any other type if necessary
    template<typename T>
    void add(T value) {
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    void endline() {
        if (file) {
            file.println();
            file.flush();  // Ensures data is written to the card
        }
    }

    void close() {
        if (file) {
            file.close();
        }
    }

private:
    int chipSelect;
    const char *fileName;
    File file;
};

#endif
