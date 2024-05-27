#ifndef POT_H
#define POT_H

#include <Arduino.h>
#include <MIDI.h>
#include "muxer.h"
#include "helpers.h"

const int DEFUZZ_BUFFER = 5;

class Pot {
  private:
    Muxer muxer;
    byte muxerChannel;
    byte ccNumber;
    byte channel;
    byte outputMin;
    byte outputMax;
    int inputMin;
    int inputMax;

    int prevReadValue = 0;
    int prevValue = 0;

  public:
    Pot(
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
