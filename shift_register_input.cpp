#include "shift_register_input.h"

ShiftRegisterInput::ShiftRegisterInput(
  byte signalPin,
  byte clockPin,
  byte latchPin,
  int count
):
  signalPin(signalPin),
  clockPin(clockPin),
  latchPin(latchPin),
  count(count),
  curData(new byte[count]),
  prevData(new byte[count])
{};

ShiftRegisterInput::~ShiftRegisterInput() {
  delete[] curData;
  delete[] prevData;
};

void ShiftRegisterInput::begin() {
  pinMode(signalPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  // Init data
  for (int i = 0; i < count; i ++) {
    curData[i] = 0;
    prevData[i] = 0;
  }
};

void ShiftRegisterInput::update() {
  delete[] prevData;
  prevData = curData;
  curData = new byte[count];

  digitalWrite(latchPin, LOW);
  delayMicroseconds(5);
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(5);
  
  for (int i = 0; i < count; i ++) {
    curData[i] = 0;
    for (int j = 0; j < 8; j ++) {
      int val = digitalRead(signalPin);

      if (val) {
        curData[i] = curData[i] | 1 << j;
      }

      digitalWrite(clockPin, HIGH);
      delayMicroseconds(2);
      digitalWrite(clockPin, LOW);
    }
  }
};

bool ShiftRegisterInput::wasSwitchedOn(int index) {
  int i = index / 8;
  byte bit = index % 8;

  return (curData[i] & (1 << bit)) && !(prevData[i] & (1 << bit));
};

bool ShiftRegisterInput::wasSwitchedOff(int index) {
  int i = index / 8;
  byte bit = index % 8;

  return !(curData[i] & (1 << bit)) && (prevData[i] & (1 << bit));
};

bool ShiftRegisterInput::getCurrentState(int index) {
  int i = index / 8;
  byte bit = index % 8;

  return curData[i] & (1 << bit);
};
