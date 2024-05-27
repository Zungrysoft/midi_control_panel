#include "pot.h"

Pot::Pot(
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
  inputMax(inputMax)
{};

void Pot::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  int readValue = muxer.read(muxerChannel);
  int value = map(readValue, inputMin, inputMax, outputMin, outputMax, true);

  if (value != prevValue) {
    // Read value must be at least 3 away from previous (prevents indecisive shifting between values)
    if (abs(readValue - prevReadValue) >= DEFUZZ_BUFFER || value == outputMin || value == outputMax) {
      // Send Control Change
      midiInstance.sendControlChange(ccNumber, value, channel);

      prevReadValue = readValue;
      prevValue = value;
    }
  }
};

void Pot::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  int readValue = muxer.read(muxerChannel);
    int value = map(readValue, inputMin, inputMax, outputMin, outputMax, true);

    // Send Control Change
    midiInstance.sendControlChange(ccNumber, value, channel);

    prevReadValue = readValue;
    prevValue = value;
}
