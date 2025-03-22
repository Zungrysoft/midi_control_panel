#include "control_leslie_switch.h"

ControlLeslieSwitch::ControlLeslieSwitch(
  byte speedPin,
  byte slowModePin,
  byte indicatorLightPin,
  byte ccNumber,
  byte channel,
  byte outputStop,
  byte outputSlow,
  byte outputFast
):
  speedPin(speedPin),
  slowModePin(slowModePin),
  indicatorLightPin(indicatorLightPin),
  ccNumber(ccNumber),
  channel(channel),
  outputStop(outputStop),
  outputSlow(outputSlow),
  outputFast(outputFast),
  switchObjectSpeed(Switch(speedPin)),
  switchObjectSlowMode(Switch(slowModePin)),
  indicatorLightObject(IndicatorLight(indicatorLightPin))
{};

int ControlLeslieSwitch::getCurrentSpeed() {
  if (state) {
    return 2; // Fast
  }
  if (switchObjectSlowMode.getCurrentState()) {
    return 1; // Slow
  }
  return 0; // Stopped
};

void ControlLeslieSwitch::simulateRotation(int selectedSpeed) {
  long int time = millis();
  float timeDelta = (time - prevTime) / 1000.0;
  prevTime = time;

  // Determine desired speed
  float desiredSpeed = LESLIE_SPEED_STOP;
  if (selectedSpeed == 1) {
    desiredSpeed = LESLIE_SPEED_SLOW;
  }
  if (selectedSpeed == 2) {
    desiredSpeed = LESLIE_SPEED_FAST;
  }

  // Accelerate or decelerate speed to desired
  if (rotationSpeed < desiredSpeed) {
    rotationSpeed = min(rotationSpeed + LESLIE_ACCELERATION * timeDelta, desiredSpeed);
  }
  rotationSpeed *= pow(1.0 - LESLIE_FRICTION, timeDelta);

  // Rotate by speed
  rotationPosition += rotationSpeed * timeDelta;
  if (rotationPosition > TWO_PI) {
     if (selectedSpeed == 0 && rotationSpeed <= desiredSpeed) {
        rotationPosition = TWO_PI;
     }
     else {
      rotationPosition -= TWO_PI;
     }
  }

  // Set light value based on position
  float lightValue = pow(mapFloat(cos(rotationPosition), -1.0, 1.0, 0.0, 1.0), 2.4);
  indicatorLightObject.setValue(lightValue);
};

void ControlLeslieSwitch::begin() {
  switchObjectSpeed.begin();
  switchObjectSlowMode.begin();
  indicatorLightObject.begin();
  prevSpeed = getCurrentSpeed();
  indicatorLightObject.setState(true);
};

void ControlLeslieSwitch::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  switchObjectSpeed.update();
  switchObjectSlowMode.update();

  // Toggle momentary switch
  if (switchObjectSpeed.wasSwitchedOn()) {
    state = !state;
  }

  // CC message
  int speed = getCurrentSpeed();
  if (speed != prevSpeed) {
    int outputValue = outputStop;
    if (speed == 1) {
      outputValue = outputSlow;
    }
    else if (speed == 2) {
      outputValue = outputFast;
    }
    midiInstance.sendControlChange(ccNumber, outputValue, channel);
  }
  prevSpeed = speed;

  // Simulate rotation for indicator light
  simulateRotation(speed);

  indicatorLightObject.update();
};

void ControlLeslieSwitch::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  int speed = getCurrentSpeed();
  int outputValue = outputStop;
  if (speed == 1) {
    outputValue = outputSlow;
  }
  else if (speed == 2) {
    outputValue = outputFast;
  }
  midiInstance.sendControlChange(ccNumber, outputValue, channel);
}

void ControlLeslieSwitch::setBrightness(int brightness) {
  indicatorLightObject.setBrightness(brightness);
}
