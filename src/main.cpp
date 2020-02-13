#include <Arduino.h>

// code genrated with Audio System Design Tool for Teensy Audio Library
// www.pjrc.com/teensy/gui

#include <Audio.h>
// NOTE are the libraries below needed?
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=320,151
AudioAnalyzeFFT1024      fft1024_1;      //xy=496,151
AudioConnection          patchCord1(adc1, fft1024_1);
// GUItool: end automatically generated code

// ---- end of code generated by the Audio System Design Tool

// AudioConnection objects do not have any functions.
// They are simply created in your sketch, after the audio objects, to define the data flow between those objects.

// some variables
float magnitude = 0;
float dB;

unsigned long previousMillis = 0;
unsigned long samplingInterval = 250;  //in ms

void setup() {
  // put your setup code here, to run once:

  // ADC resolution defaults to 10 bits for Arduino compatibility.
  // https://www.arduino.cc/reference/en/language/functions/zero-due-mkr-family/analogreadresolution/
  // set to 16bits
  analogReadResolution(16);

  // Allocate the memory for all audio connections.
  // The numberBlocks input specifies how much memory to reserve for audio data. Each block holds 128 audio samples
  AudioMemory(10);  // tested with AudioMemoryUsageMax()

  // Configure the window algorithm to use, for avoiding spectral leakage effect
  fft1024_1.windowFunction(AudioWindowHanning1024);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  // TODO add millis interval for sys info
  //monitoring system usage
  // Serial.print("Max audio mem used: ");
  // Serial.print(AudioMemoryUsageMax());
  // Serial.print(" cpu: ");
  // Serial.println(AudioProcessorUsageMax());
  // delay(500);


  // ---------  SPL from FFT -----------------------
  // - apply window function (e.g. Hann or Hamming)
  // - calculate FFT
  // - calculate magnitude of FFT (sqrt(re*re+im*im))
  // - convert magnitude to dB (20*log10(magnitude))

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > samplingInterval) {
    previousMillis = currentMillis;

    if (fft1024_1.available()) {
      float v[512] = {0};
      magnitude = 0;

      for (int i=0; i<512; i++){
        v[i] = fft1024_1.read(i); // Read frequency bins. The result is scaled so 1.0 represents a full scale sine wave.
        // The term bins is related to the result of the FFT, where every element in the result array is a bin.
        // One can say this is the “resolution” of the FFT. Every bin represent a frequency interval, just like a histogram.
        // The number of bins you get is half the amount of samples spanning the frequency range from zero to half the sampling rate
        magnitude = magnitude + sq(v[i]); //
      } // end of for

      magnitude = sqrt(magnitude);
      dB = log10f(magnitude) * 20  + 125.05;  // db = 20(log A/Aref)
      Serial.print("db = ");
      Serial.println(dB,2);
    } // end of if fft

  } // end of if millis

}
