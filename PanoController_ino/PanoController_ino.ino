// Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit

// which analog pin to connect
#define THERMISTORPIN_0 A0
#define THERMISTORPIN_1 A1
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 20
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
// 10k resistor
#define SERIESRESISTOR 10000    

int samples1[NUMSAMPLES];
int samples2[NUMSAMPLES];

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop(void) {
  uint8_t i;
  float average1;
  float average2;

  // take N samples in a row, with a a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples1[i] = analogRead(THERMISTORPIN_0);
   samples2[i] = analogRead(THERMISTORPIN_1);
   delay(10);
  }
  
  // average all the samples out
  average1 = 0;
  average2 = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average1 += samples1[i];
     average2 += samples2[i];
  }
  average1 /= NUMSAMPLES;
  average2 /= NUMSAMPLES;

  Serial.print("Average analog reading "); 
  Serial.println(average1);
  Serial.println(average2);
  
  // convert the value to resistance
  average1 = 1023 / average1 - 1;
  average1 = SERIESRESISTOR / average1;
  average2 = 1023 / average2 - 1;
  average2 = SERIESRESISTOR / average2;
  Serial.print("Thermistor resistance "); 
  Serial.println(average1);
  Serial.println(average2);
  
  float steinhart1;
  steinhart1 = average1 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart1 = log(steinhart1);                  // ln(R/Ro)
  steinhart1 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart1 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart1 = 1.0 / steinhart1;                 // Invert
  steinhart1 -= 273.15;                         // convert to C
  
  float steinhart2;
  steinhart2 = average2 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart2 = log(steinhart2);                  // ln(R/Ro)
  steinhart2 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart2 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart2 = 1.0 / steinhart2;                 // Invert
  steinhart2 -= 273.15;                         // convert to C
  
  Serial.print("Temperature 1: "); 
  Serial.print(steinhart1);
  Serial.println(" *C");
  Serial.print("Temperature 2: ");
  Serial.print(steinhart2);
  Serial.println(" *C");
  
  delay(1000);
}

