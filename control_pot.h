#ifndef CONTROL_POT_H
#define CONTROL_POT_H

#include <Arduino.h>
#include <MIDI.h>
#include "pot.h"
#include "muxer.h"
#include "helpers.h"

class ControlPot {
  private:
    Muxer muxer;
    byte muxerChannel;
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
      Muxer muxer,
      byte muxerChannel,
      byte ccNumber,
      byte channel,
      byte outputMin,
      byte outputMax,
      int inputMin,
      int inputMax
    );
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
};

#endif
