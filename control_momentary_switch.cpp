#include "control_momentary_switch.h"

ControlMomentarySwitch::ControlMomentarySwitch(
  byte pin,
  byte indicatorLightPin,
  byte ccNumber,
  byte channel,
  byte outputMin,
  byte outputMax
):
  pin(pin),
  indicatorLightPin(indicatorLightPin),
  ccNumber(ccNumber),
  channel(channel),
  outputMin(outputMin),
  outputMax(outputMax),
  state(false),
  switchObject(Switch(pin)),
  indicatorLightObject(IndicatorLight(indicatorLightPin))
{};

void ControlMomentarySwitch::begin() {
  switchObject.begin();
  indicatorLightObject.begin();
}

void ControlMomentarySwitch::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  switchObject.update();

  // Toggled
  if (switchObject.wasSwitchedOn()) {
    state = !state;
    midiInstance.sendControlChange(ccNumber, state ? outputMax : outputMin, channel);
    indicatorLightObject.setState(state);
  }

  indicatorLightObject.update();
};

void ControlMomentarySwitch::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  midiInstance.sendControlChange(ccNumber, state ? outputMax : outputMin, channel);
}

void ControlMomentarySwitch::setBrightness(int brightness) {
  indicatorLightObject.setBrightness(brightness);
}
