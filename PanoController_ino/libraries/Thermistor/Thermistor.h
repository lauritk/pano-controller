#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"

class Thermistor
{
  public:
    Thermistor(byte pin, unsigned long nominalRes, int nominalTemp,
      unsigned int bCoeff, unsigned long seriesRes);
    float read(int numSamples);
  private:
    byte pin;
    unsigned long nominalRes;
    int nominalTemp;
    unsigned int bCoeff;
    unsigned long seriesRes;
    float steinhart(float value);
};

#endif
