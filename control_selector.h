#ifndef CONTROL_SELECTOR_H
#define CONTROL_SELECTOR_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"
#include "shift_register_input.h"
#include "shift_register_output.h"

class ControlSelector : public Control {
  private:
    ShiftRegisterInput* shiftRegisterInput;
    ShiftRegisterOutput* shiftRegisterOutput;
    int numSelections;
    int* inputIndices;
    int* outputIndices;
    int* ccNumbers;
    int* channels;

    int curSelection = 0;
    int prevSelection = 0;

  public:
    ControlSelector(
      ShiftRegisterInput* shiftRegisterInput,
      ShiftRegisterOutput* shiftRegisterOutput,
      int numSelections,
      int* inputIndices,
      int* outputIndices,
      int* ccNumbers,
      int* channels
    );
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    
};

#endif
