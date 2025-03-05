#include "control_pot.h"

ControlPot::ControlPot(
  Muxer* muxer,
  byte pin,
  byte ccNumber,
  byte channel,
  byte outputMin,
  byte outputMax,
  int inputMin,
  int inputMax
):
  muxer(muxer),
  pin(pin),
  ccNumber(ccNumber),
  channel(channel),
  outputMin(outputMin),
  outputMax(outputMax),
  inputMin(inputMin),
  inputMax(inputMax),
  potObject(Pot(muxer, pin))
{};

ControlPot::ControlPot(
  byte pin,
  byte ccNumber,
  byte channel,
  byte outputMin,
  byte outputMax,
  int inputMin,
  int inputMax
):
  muxer(nullptr),
  pin(pin),
  ccNumber(ccNumber),
  channel(channel),
  outputMin(outputMin),
  outputMax(outputMax),
  inputMin(inputMin),
  inputMax(inputMax),
  potObject(Pot(pin))
{};

void ControlPot::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  potObject.update();
  int readValue = potObject.getCurrentValue();
  byte value = constrain(map(readValue, inputMin, inputMax, outputMin, outputMax), 0, 127);

  // If value was changed...
  if (value != prevValue) {
    if (potObject.wasValueChanged() || value == outputMin || value == outputMax) {
      // Send Control Change
      midiInstance.sendControlChange(ccNumber, value, channel);
      prevValue = value;
    }
  }
};

void ControlPot::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  int readValue = potObject.getCurrentValue();
  byte value = constrain(map(readValue, inputMin, inputMax, outputMin, outputMax), 0, 127);

  // Send Control Change
  midiInstance.sendControlChange(ccNumber, value, channel);
  prevValue = value;
}
