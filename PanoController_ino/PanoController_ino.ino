#include <Thermistor.h>
#include <RCSwitch.h>
#include <PID_v1.h>
// Add PID autotune later: https://playground.arduino.cc/Code/PIDAutotuneLibrary/

#define THERMISTOR0_PIN A2

RCSwitch mySwitch = RCSwitch();

Thermistor temp1 = Thermistor(THERMISTOR0_PIN, 10000, 25, 3950, 10000);

double Setpoint = 28.0, Input, Output;

// Defaults taken from https://github.com/Brewzone/TFTBREW/blob/master/TFT_BREW.ino
double Kp = 1224.0, Ki = 2524.5, Kd = 1198.5;

PID controlPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

bool heating = false;
bool cooling = false;

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);

  mySwitch.enableTransmit(8);
  mySwitch.switchOff(1, 1);
  mySwitch.switchOff(1, 2);

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

  Serial.print(Input);
  Serial.print(" C");
  Serial.println();

  if (Output > now - windowStartTime) {
    if (controlPID.GetDirection() == REVERSE) {
      Serial.println("Cooling");
      if (heating) {
        mySwitch.switchOff(1, 1);
        heating = false;
      }
      if (!cooling) {
        mySwitch.switchOn(1, 2);
        cooling = true;
      }
    } else {
      Serial.println("Heating");
      if (cooling) {
        mySwitch.switchOff(1, 2);
        cooling = false;
      }
      if (!heating) {
        mySwitch.switchOn(1, 1);
        heating = true;
      }

    }
  } else {
    Serial.print(Setpoint);
    Serial.print("C target reach!");
    Serial.println();
    if (heating || cooling) {
      mySwitch.switchOff(1, 1);
      mySwitch.switchOff(1, 2);
      heating = false;
      cooling = false;
    }
  }

}
