

#include <arduinoFFT.h>

#include <Wire.h>
#include <LSM6DS3.h>


LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

// FFT parameters
#define SAMPLES 64             // Must be a power of 2 for FFT (more samples for better accuracy)
#define SAMPLING_FREQUENCY 2    // Hz (Sample rate of the IMU)
#define FFT_SIZE SAMPLES / 2     // FFT output size is half the input size
#define TIME_INTERVAL 1.0 / SAMPLING_FREQUENCY // Time step for sampling (50 Hz)

float zAccel[SAMPLES];            // Array to store Z-axis acceleration
float vReal[SAMPLES];             // Real part for FFT
float vImag[SAMPLES];             // Imaginary part for FFT
float displacements[SAMPLES];     // Array to store displacement after processing

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Variables for wave height calculation
float waveHeights[SAMPLES];       // Stores wave heights estimated from displacement
int sampleCount = 0;



void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);
    //Call .begin() to configure the IMUs
    if (myIMU.begin() != 0) {
        Serial.println("Device error");
    } else {
        Serial.println("Device OK!");
    }
}

// Main loop for reading IMU data and calculating significant wave height
void loop() {
  if (sampleCount < SAMPLES) {
    // Read the Z-axis acceleration from the IMU
    zAccel[sampleCount] = myIMU.readFloatAccelZ();

    // Store data for FFT processing
    vReal[sampleCount] = zAccel[sampleCount];
    vImag[sampleCount] = 0;  // Imaginary part set to 0
    
    sampleCount++;
  } else {
    // Once enough data is collected, apply FFT
    applyFFT();

    // Convert acceleration to displacement
    convertAccelerationToDisplacement();

    // Calculate the significant wave height
    float Hs = calculateSignificantWaveHeight();

    // Output the significant wave height
    Serial.print("Significant Wave Height (Hs): ");
    Serial.println(Hs);

    // Reset sample count for the next batch
    sampleCount = 0;
  }
}

// Function to apply FFT for noise reduction
void applyFFT() {
  // Apply a windowing function to reduce spectral leakage
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  
  // Perform FFT computation
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);

  // Convert complex numbers to magnitudes
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);
}

// Function to convert acceleration to displacement (simple integration)
void convertAccelerationToDisplacement() {
  displacements[0] = 0;  // Initial displacement set to zero

  // Simple numerical integration of acceleration to displacement
  for (int i = 1; i < SAMPLES; i++) {
    float velocity = 0.5 * (zAccel[i] + zAccel[i - 1]) * TIME_INTERVAL;  // Trapezoidal integration for velocity
    displacements[i] = displacements[i - 1] + velocity * TIME_INTERVAL;  // Displacement integration
  }
}

// Function to calculate significant wave height (Hs)
float calculateSignificantWaveHeight() {
  // Convert displacements to wave heights by taking the peak-to-peak values
  for (int i = 0; i < SAMPLES - 1; i++) {
    waveHeights[i] = abs(displacements[i + 1] - displacements[i]);
  }

  // Sort the wave heights array in descending order
  sortDescending(waveHeights, SAMPLES);

  // Compute Hs by averaging the highest one-third of wave heights
  int topThirdCount = SAMPLES / 3;
  float sum = 0;
  for (int i = 0; i < topThirdCount; i++) {
    sum += waveHeights[i];
  }
  
  // Return the average of the highest one-third wave heights
  return sum / topThirdCount;
}

// Helper function to sort an array in descending order
void sortDescending(float* array, int length) {
  for (int i = 0; i < length - 1; i++) {
    for (int j = i + 1; j < length; j++) {
      if (array[i] < array[j]) {
        float temp = array[i];
        array[i] = array[j];
        array[j] = temp;
      }
    }
  }
}
