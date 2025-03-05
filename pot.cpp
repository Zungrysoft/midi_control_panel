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

void Pot::update() {
  const int readValue = muxer ? muxer->read(pin) : analogRead(pin);

   // Read value must be at least 5 away from previous to update (prevents indecisive shifting between values)
  if (abs(readValue - curValue) >= DEFUZZ_BUFFER) {
    curValue = readValue;
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
