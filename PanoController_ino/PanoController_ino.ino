#include <Thermistor.h>
#include <PID_v1.h>

#define THERMISTOR0_PIN A2
#define THERMISTOR1_PIN A3
#define HEATER_PIN 3
#define COOLER_PIN 4

Thermistor temp1 = Thermistor(THERMISTOR0_PIN, 10000, 25, 3950, 10000);

double Setpoint, Input, Output;

//Aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

PID controlPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(COOLER_PIN, OUTPUT);

  windowStartTime = millis();

  Input = temp1.read(10);
  Setpoint = 27;

  controlPID.SetOutputLimits(0, WindowSize);
  controlPID.SetMode(AUTOMATIC);
}

void loop(void) {
  Input = temp1.read(10);

  double gap = abs(Setpoint-Input);
  if (Input > Setpoint + 2) {
    controlPID.SetControllerDirection(REVERSE);
  } else {
    controlPID.SetControllerDirection(DIRECT);
  }
  if (gap < 10)
  {
    controlPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
    controlPID.SetTunings(aggKp, aggKi, aggKd);
  }

  controlPID.Compute();

  unsigned long now = millis();
  if (now - windowStartTime > WindowSize)
  {
    windowStartTime += WindowSize;
  }

  Serial.println(Input);

  if (Output > now - windowStartTime) {
    if (controlPID.GetDirection() == REVERSE) {
      Serial.println("Cooling");
      digitalWrite(HEATER_PIN, HIGH);
      digitalWrite(COOLER_PIN, HIGH);
    } else {
      Serial.println("Heating");
      digitalWrite(COOLER_PIN, LOW);
      digitalWrite(HEATER_PIN, HIGH);
    }
  } else {
    digitalWrite(HEATER_PIN, LOW);
    digitalWrite(COOLER_PIN, LOW);
  }
  
}
