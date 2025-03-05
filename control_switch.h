#ifndef CONTROL_SWITCH_H
#define CONTROL_SWITCH_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"
#include "switch.h"

class ControlSwitch : public Control {
  private:
    byte pin;
    byte ccNumber;
    byte channel;
    byte outputMin;
    byte outputMax;

    Switch switchObject;

  public:
    ControlSwitch(
      byte pin,
      byte ccNumber,
      byte channel,
      byte outputMin,
      byte outputMax
    );
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
};

#endif
