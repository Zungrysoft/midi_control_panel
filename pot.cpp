#include "pot.h"

Pot::Pot(
  Muxer muxer,
  byte muxerChannel
):
  muxer(muxer),
  muxerChannel(muxerChannel)
{};

void Pot::update() {
  const int readValue = muxer.read(muxerChannel);

   // Read value must be at least 5 away from previous (prevents indecisive shifting between values)
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
