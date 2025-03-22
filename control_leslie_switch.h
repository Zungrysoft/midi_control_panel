#ifndef CONTROL_LESLIE_SWITCH_H
#define CONTROL_LESLIE_SWITCH_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"
#include "switch.h"
#include "indicator_light.h"
#include "helpers.h"

#define LESLIE_SPEED_FAST 26.18
#define LESLIE_SPEED_SLOW 3.87
#define LESLIE_SPEED_STOP 1.9
#define LESLIE_ACCELERATION 17
#define LESLIE_FRICTION 0.4

class ControlLeslieSwitch : public Control {
  private:
    byte speedPin;
    byte slowModePin;
    byte indicatorLightPin;
    byte ccNumber;
    byte channel;
    byte outputStop;
    byte outputSlow;
    byte outputFast;

    byte prevSpeed;
    byte state = false;
    Switch switchObjectSpeed;
    Switch switchObjectSlowMode;
    IndicatorLight indicatorLightObject;
    long unsigned int prevTime = 0;
    float rotationPosition = PI;
    float rotationSpeed = 0;

    int getCurrentSpeed();
    void simulateRotation(int selectedSpeed);

  public:
    ControlLeslieSwitch(
      byte speedPin,
      byte slowModePin,
      byte indicatorLightPin,
      byte ccNumber,
      byte channel,
      byte outputStop,
      byte outputSlow,
      byte outputFast
    );
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void setBrightness(int brightness);
};

#endif
