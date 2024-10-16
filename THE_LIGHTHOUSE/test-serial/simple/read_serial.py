import serial
import time

# Open the serial port (adjust the COM port name and baudrate as necessary)
ser = serial.Serial('/dev/cu.usbserial-110', 9600, timeout=1)  # Replace '/dev/cu.usbserial-110' with the correct port on your system

# Give some time for the connection to establish
time.sleep(2)

def parse_data(line):
    if line.startswith("DATA:"):
        print(f"Received Data: {line[5:].strip()}")
    elif line.startswith("RSSI:"):
        print(f"RSSI: {line[5:].strip()} dBm")
    elif line.startswith("SNR:"):
        print(f"SNR: {line[4:].strip()} dB")
    else:
        print(f"Other output: {line}")

try:
    while True:
        # Read line from the serial port
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            parse_data(line)
        
except KeyboardInterrupt:
    print("Exiting...")
    
finally:
    # Close the serial connection
    ser.close()
