#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <MIDI.h>

class Control {
  public:
    virtual void begin() {};
    virtual void update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) = 0;
    virtual void forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) = 0;
};

#endif
