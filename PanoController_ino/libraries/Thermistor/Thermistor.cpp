#include "Arduino.h"
#include "Thermistor.h"

Thermistor::Thermistor(byte pin, unsigned long nominalRes, int nominalTemp,
      unsigned int bCoeff, unsigned long seriesRes)
{
  this->pin = pin;
  this->nominalRes = nominalRes;
  this->nominalTemp = nominalTemp;
  this->bCoeff = bCoeff;
  this->seriesRes = seriesRes;
}

float Thermistor::read(int numSamples=1)
{
  int i;
  float avg;
  float avgResistance;
  int samples[numSamples];

  for (i = 0; i < numSamples; i++) {
    samples[i] = analogRead(this->pin);
    delay(10);
  }

  avg = 0;
  for (i = 0; i < numSamples; i++) {
    avg += samples[i];
  }
  avg /= numSamples;

  avgResistance = 1023 / avg - 1;
  avgResistance = this->seriesRes / avgResistance;

  return steinhart(avgResistance);
}

float Thermistor::steinhart(float value)
{
  float celsius;
  celsius = value / this->nominalRes;             // (R/Ro)
  celsius = log(celsius);                         // ln(R/Ro)
  celsius /= this->bCoeff;                        // 1/B * ln(R/Ro)
  celsius += 1.0 / (this->nominalTemp + 273.15);  // + (1/To)
  celsius = 1.0 / celsius;                        // Invert
  celsius -= 273.15;                              // convert to C
  return celsius;
}
