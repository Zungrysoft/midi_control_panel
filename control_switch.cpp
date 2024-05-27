#include "control_switch.h"

ControlSwitch::ControlSwitch(
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
  outputMax(outputMax),
  switchObject(Switch(pin))
{};

void ControlSwitch::begin() {
  switchObject.begin();
}

void ControlSwitch::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  switchObject.update();

  // Switched on
  if (switchObject.wasSwitchedOn()) {
    midiInstance.sendControlChange(ccNumber, outputMax, channel);
  }

  // Switched off
  if (switchObject.wasSwitchedOff()) {
    midiInstance.sendControlChange(ccNumber, outputMin, channel);
  }
};

void ControlSwitch::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  if (switchObject.getCurrentState()) {
    midiInstance.sendControlChange(ccNumber, outputMax, channel);
  }
  else {
    midiInstance.sendControlChange(ccNumber, outputMin, channel);
  }
}
