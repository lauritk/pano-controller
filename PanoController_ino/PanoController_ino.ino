#define THERMISTORPIN_0 A0
#define THERMISTORPIN_1 A1
#define THERMISTORPIN_2 A2
#define THERMISTORPIN_3 A3

class Thermistor {
  private:
    byte pin;
    unsigned long nominalRes;
    int nominalTemp;
    unsigned int bCoeff;  
    unsigned long seriesRes;

    float steinhart(float value) {
      float celsius;
      celsius = value / this->nominalRes;             // (R/Ro)
      celsius = log(celsius);                         // ln(R/Ro)
      celsius /= this->bCoeff;                        // 1/B * ln(R/Ro)
      celsius += 1.0 / (this->nominalTemp + 273.15);  // + (1/To)
      celsius = 1.0 / celsius;                        // Invert
      celsius -= 273.15;                              // convert to C
      return celsius;
    }

  public:
    Thermistor(byte pin, unsigned long nominalRes, int nominalTemp,
      unsigned int bCoeff, unsigned long seriesRes) {
        this->pin = pin;
        this->nominalRes = nominalRes;
        this->nominalTemp = nominalTemp;
        this->bCoeff = bCoeff;
        this->seriesRes = seriesRes;
    }

  float read(int numSamples=1) {
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

    return this->steinhart(avgResistance);

  }


};

Thermistor temp1 = Thermistor(THERMISTORPIN_2, 10000, 25, 3950, 10000);

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop(void) {
  
  Serial.print("Temperature 1: "); 
  Serial.print(temp1.read(10));
  Serial.println(" *C");
  
  delay(1000);
}
