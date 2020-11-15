#include <Thermistor.h>
#include <RCSwitch.h>
#include <PID_v1.h>
// Add PID autotune later: https://playground.arduino.cc/Code/PIDAutotuneLibrary/

#define THERMISTOR0_PIN A2
#define THERMISTOR1_PIN A3
#define HEATER_PIN 3
#define COOLER_PIN 4

RCSwitch mySwitch = RCSwitch();

Thermistor temp1 = Thermistor(THERMISTOR0_PIN, 10000, 25, 3950, 10000);

double Setpoint = 27.0, Input, Output;

// Defaults taken from https://github.com/Brewzone/TFTBREW/blob/master/TFT_BREW.ino
double Kp = 1224.0, Ki = 2524.5, Kd = 1198.5;

PID controlPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

bool heating = false;

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);

  mySwitch.enableTransmit(8);
  mySwitch.switchOff(1, 1);
  mySwitch.switchOff(1, 2);

  // pinMode(HEATER_PIN, OUTPUT);
  // pinMode(COOLER_PIN, OUTPUT);

  windowStartTime = millis();

  Input = temp1.read(10);

  controlPID.SetOutputLimits(0, WindowSize);
  controlPID.SetMode(AUTOMATIC);
}

void loop(void) {
  Input = temp1.read(10);

  // Start cooler only if 1 degree over target. Tune this!
  if (Input > Setpoint + 4) {
    controlPID.SetControllerDirection(REVERSE);
  } else {
    controlPID.SetControllerDirection(DIRECT);
  }

  controlPID.Compute();

  unsigned long now = millis();
  while (now - windowStartTime > WindowSize)
  {
    windowStartTime += WindowSize;
  }

  Serial.println(Input);

  if (Output > now - windowStartTime) {
    if (controlPID.GetDirection() == REVERSE) {
      Serial.println("Cooling");
      // digitalWrite(HEATER_PIN, LOW);
      // digitalWrite(COOLER_PIN, HIGH);
      mySwitch.switchOn(1, 2);
      mySwitch.switchOff(1, 1);
      heating = true;
    } else {
      Serial.println("Heating");
      if (!heating) {
        // digitalWrite(COOLER_PIN, LOW);
        // digitalWrite(HEATER_PIN, HIGH);
        mySwitch.switchOn(1, 1);
        mySwitch.switchOff(1, 2);
        heating = true;
      }

    }
  } else {
    Serial.println("Target reach");
    if (heating) {
      digitalWrite(HEATER_PIN, LOW);
      digitalWrite(COOLER_PIN, LOW);
      mySwitch.switchOff(1, 1);
      mySwitch.switchOff(1, 2);
      heating = false;
    }
  }

}
