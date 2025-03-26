#ifndef CONTROL_SET_H
#define CONTROL_SET_H

#include <Arduino.h>
#include <MIDI.h>
#include "control.h"

class ControlSetNode {
  public:
    ControlSetNode* next = nullptr;
    Control* control = nullptr;

    ControlSetNode(Control* control);
};

class ControlSet {
  private:
    ControlSetNode* head = nullptr;
    ControlSetNode* tail = nullptr;

  public:
    void add(Control* control);
    void begin();
    void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance);
    void setBrightness(int brightness);

};

#endif
