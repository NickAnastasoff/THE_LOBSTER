#ifndef LOGGER_H
#define LOGGER_H

#include <SD.h>

class Logger {
public:
    Logger(int chipSelectPin, const char *filename) {
        chipSelect = chipSelectPin;
        fileName = filename;
        newLine = true;  // Indicates if it's the start of a new line
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
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for const char*
    void add(const char *value) {
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for int
    void add(int value) {
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for float
    void add(float value) {
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Overloaded add method for double
    void add(double value) {
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    // Add support for any other type if necessary
    template<typename T>
    void add(T value) {
        logTimeIfNewLine();
        if (file) {
            file.print(value);
            file.print(",");
        }
    }

    void endline() {
        if (file) {
            file.println();
            file.flush();  // Ensures data is written to the card
            newLine = true;  // Mark as start of a new line
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
    bool newLine;  // Flag to track if it is the start of a new line

    // Placeholder function to simulate getting the current time as a string
    String getTime() {
        // Replace with actual RTC code or other time source
        return "2024-10-04 12:34:56";
    }

    // Log the time if it's the start of a new line
    void logTimeIfNewLine() {
        if (newLine && file) {
            String time = getTime();  // Assuming getTime() returns a formatted time string
            file.print(time);
            file.print(", ");
            newLine = false;  // Set to false after logging the time
        }
    }
};

#endif
