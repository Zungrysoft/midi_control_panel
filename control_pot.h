#ifndef CONTROL_POT_H
#define CONTROL_POT_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"
#include "pot.h"
#include "muxer.h"

class ControlPot : public Control {
  private:
    Muxer* muxer;
    byte pin;
    byte ccNumber;
    byte channel;
    byte outputMin;
    byte outputMax;
    int inputMin;
    int inputMax;

    Pot potObject;
    int prevValue = 0;

  public:
    ControlPot(
      Muxer* muxer,
      byte pin,
      byte ccNumber,
      byte channel,
      byte outputMin,
      byte outputMax,
      int inputMin,
      int inputMax
    );
    ControlPot(
      byte pin,
      byte ccNumber,
      byte channel,
      byte outputMin,
      byte outputMax,
      int inputMin,
      int inputMax
    );
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
};

#endif
