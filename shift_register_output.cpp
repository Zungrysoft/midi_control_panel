#include "shift_register_output.h"

ShiftRegisterOutput::ShiftRegisterOutput(
  byte signalPin,
  byte clockPin,
  byte latchPin,
  byte outputEnablePin,
  int count
):
  signalPin(signalPin),
  clockPin(clockPin),
  latchPin(latchPin),
  outputEnablePin(outputEnablePin),
  count(count),
  data(new byte[count])
{};

ShiftRegisterOutput::~ShiftRegisterOutput() {
  delete[] data;
};

void ShiftRegisterOutput::begin() {
  pinMode(signalPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);

  analogWrite(outputEnablePin, 0);

  // Init data
  for (int i = 0; i < count; i ++) {
    data[i] = 0;
  }
};

void ShiftRegisterOutput::update() {
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < count; i ++) {
    shiftOut(signalPin, clockPin, LSBFIRST, data[i]);
  }

  digitalWrite(latchPin, HIGH);
};

void ShiftRegisterOutput::setBrightness(int brightness) {
  analogWrite(outputEnablePin, 255 - brightness);
}

void ShiftRegisterOutput::write(int index, bool state) {
  int i = index / 8;
  byte bit = index % 8;

  if (state) {
    data[i] = data[i] | (1 << bit);
  }
  else {
    data[i] = data[i] & (~(1 << bit));
  }
  
};
