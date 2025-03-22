#include "pot.h"

Pot::Pot(
  Muxer* muxer,
  byte pin
):
  muxer(muxer),
  pin(pin)
{};

Pot::Pot(
  byte pin
):
  muxer(nullptr),
  pin(pin)
{};

int Pot::readFromPin() {
  return muxer ? muxer->read(pin) : analogRead(pin);
};

void Pot::begin() {
  const int readValue = readFromPin();
  for (int i = 0; i < TIME_AVERAGING_WINDOW; i ++) {
    readValues[i] = readValue;
  }
};

void Pot::update() {
  const int readValue = readFromPin();

  // Perform time averaging
  int total = readValue;
  for (int i = 0; i < TIME_AVERAGING_WINDOW; i ++) {
    total += readValues[i];
    readValues[i] = i == 0 ? readValue : readValues[i-1];
  }
  int averageValue = total / (TIME_AVERAGING_WINDOW + 1);

  // Read value must be at least 5 away from previous to update (prevents indecisive shifting between values)
  if (abs(averageValue - curValue) >= DEFUZZ_BUFFER) {
    curValue = averageValue;
    valueChanged = true;
  }
  else {
    valueChanged = false;
  }
};

bool Pot::wasValueChanged() {
  return valueChanged;
}

int Pot::getCurrentValue() {
  return curValue;
}
