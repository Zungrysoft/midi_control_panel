#include "control_pot.h"

ControlPot::ControlPot(
  Muxer muxer,
  byte muxerChannel,
  byte ccNumber,
  byte channel,
  byte outputMin,
  byte outputMax,
  int inputMin,
  int inputMax
):
  muxer(muxer),
  muxerChannel(muxerChannel),
  ccNumber(ccNumber),
  channel(channel),
  outputMin(outputMin),
  outputMax(outputMax),
  inputMin(inputMin),
  inputMax(inputMax),
  potObject(Pot(muxer, muxerChannel))
{};

void ControlPot::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  int readValue = potObject.getCurrentValue();
  int value = map(readValue, inputMin, inputMax, outputMin, outputMax, true);

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
  int value = map(readValue, inputMin, inputMax, outputMin, outputMax, true);

  // Send Control Change
  midiInstance.sendControlChange(ccNumber, value, channel);
  prevValue = value;
}
