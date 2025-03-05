#include "control_dial.h"

const int CONFIRM_WINDOW = 50;

ControlDial::ControlDial(
  byte pin,
  byte ccNumber,
  byte channel,
  int numPositions,
  int* inputValues,
  int* outputValues
):
  pin(pin),
  ccNumber(ccNumber),
  channel(channel),
  numPositions(numPositions),
  inputValues(inputValues),
  outputValues(outputValues)
{};

int ControlDial::getDialPosition() {
  const int readValue = analogRead(pin);

  for (int i = 0; i < numPositions - 1; i ++) {
    int avg = (inputValues[i] + inputValues[i + 1]) / 2;
    
    if (readValue < avg) {
      return i;
    }
  }

  return numPositions - 1;
}

void ControlDial::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  // Get current dial position
  const int dialPosition = getDialPosition();
  const int value = outputValues[dialPosition];

  // If value was changed...
  if (value != prevValue) {

    // Count how long it has been since this value changed
    if (value != confirmingValue) {
      confirmingTime = millis();
      confirmingValue = value;
    }

    // If this value has been steady for long enough, Send MIDI signal
    if (millis() - confirmingTime > CONFIRM_WINDOW) {
      midiInstance.sendControlChange(ccNumber, value, channel);
      prevValue = value;
    }
  }
};

void ControlDial::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  // Get current dial position
  const int dialPosition = getDialPosition();
  const int value = outputValues[dialPosition];

  // Send Control Change
  midiInstance.sendControlChange(ccNumber, value, channel);
  prevValue = value;
}
