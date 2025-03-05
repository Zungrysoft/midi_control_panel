#ifndef CONTROL_DIAL_H
#define CONTROL_DIAL_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"

/*
To save digital pins, I implemented this six-way digital dial using
a single analog pin and voltage dividers. Each of the six positions
has a different analogRead value which are specified via inputValues
These inputs are mapped to CC outputs via the outputValues array.
*/

class ControlDial : public Control {
  private:
    byte pin;
    byte ccNumber;
    byte channel;
    int numPositions;
    int* inputValues;
    int* outputValues;

    int prevValue = 0;
    int confirmingValue = 0;
    unsigned long confirmingTime = 0;

  public:
    ControlDial(
      byte pin,
      byte ccNumber,
      byte channel,
      int numPositions,
      int* inputValues,
      int* outputValues
    );
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    int getDialPosition();
};

#endif
