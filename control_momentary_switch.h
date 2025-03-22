#ifndef CONTROL_MOMENTARY_SWITCH_H
#define CONTROL_MOMENTARY_SWITCH_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"
#include "switch.h"
#include "indicator_light.h"

class ControlMomentarySwitch : public Control {
  private:
    byte pin;
    byte indicatorLightPin;
    byte ccNumber;
    byte channel;
    byte outputMin;
    byte outputMax;

    byte state;
    Switch switchObject;
    IndicatorLight indicatorLightObject;

  public:
    ControlMomentarySwitch(
      byte pin,
      byte indicatorLightPin,
      byte ccNumber,
      byte channel,
      byte outputMin,
      byte outputMax
    );
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void setBrightness(int brightness);
};

#endif
