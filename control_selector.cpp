#include "control_selector.h"

ControlSelector::ControlSelector(
  ShiftRegisterInput* shiftRegisterInput,
  ShiftRegisterOutput* shiftRegisterOutput,
  int numSelections,
  int* inputIndices,
  int* outputIndices,
  int* ccNumbers,
  int* channels
):
  shiftRegisterInput(shiftRegisterInput),
  shiftRegisterOutput(shiftRegisterOutput),
  numSelections(numSelections),
  inputIndices(inputIndices),
  outputIndices(outputIndices),
  ccNumbers(ccNumbers),
  channels(channels)
{};

void ControlSelector::begin() {
  for (int i = 0; i < numSelections; i ++) {
    shiftRegisterOutput->write(outputIndices[i], i == curSelection);
  }
};

void ControlSelector::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  // See if selection should be changed
  for (int i = 0; i < numSelections; i ++) {
    int index = inputIndices[i];
    if (shiftRegisterInput->wasSwitchedOn(index)) {
      curSelection = i;
      break;
    }
  }

  if (curSelection != prevSelection) {
    // Switch on current selection
    shiftRegisterOutput->write(outputIndices[curSelection], true);
    midiInstance.sendControlChange(ccNumbers[curSelection], 127, channels[curSelection]);

    // Switch off previous selection
    shiftRegisterOutput->write(outputIndices[prevSelection], false);
    midiInstance.sendControlChange(ccNumbers[prevSelection], 0, channels[prevSelection]);
  }

  prevSelection = curSelection;
};

void ControlSelector::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  for (int i = 0; i < numSelections; i ++) {
    int value = i == curSelection ? 127 : 0;
    midiInstance.sendControlChange(ccNumbers[i], value, channels[i]);
  }
}
