#include "switch.h"

Switch::Switch(
  byte pin,
  byte ccNumber,
  byte channel,
  byte outputMin,
  byte outputMax
):
  pin(pin),
  ccNumber(ccNumber),
  channel(channel),
  outputMin(outputMin),
  outputMax(outputMax)
{};

void Switch::begin() {
  pinMode(pin, INPUT_PULLUP);
}

void Switch::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  bool pressed = !digitalRead(pin);

  // Switched on
  if (pressed && !prevPressed) {
    midiInstance.sendControlChange(ccNumber, outputMax, channel);
  }

  // Switched off
  if (!pressed && prevPressed) {
    midiInstance.sendControlChange(ccNumber, outputMin, channel);
  }

  prevPressed = pressed;
};

void Switch::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  
}
