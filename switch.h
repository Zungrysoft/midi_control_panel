#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <MIDI.h>

class Switch {
  private:
    byte pin;
    byte ccNumber;
    byte channel;
    byte outputMin;
    byte outputMax;

    bool prevPressed = 0;

  public:
    Switch(
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
