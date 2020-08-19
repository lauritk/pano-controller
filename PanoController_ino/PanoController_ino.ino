#include <Thermistor.h>

#define THERMISTORPIN_0 A2
#define THERMISTORPIN_1 A3

Thermistor temp1 = Thermistor(THERMISTORPIN_0, 10000, 25, 3950, 10000);

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
