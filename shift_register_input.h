#ifndef SHIFT_REGISTER_INPUT_H
#define SHIFT_REGISTER_INPUT_H

#include <Arduino.h>

class ShiftRegisterInput {
  private:
    byte signalPin;
    byte clockPin;
    byte latchPin;
    int count;

    byte* curData;
    byte* prevData;

  public:
    ShiftRegisterInput(
      byte signalPin,
      byte clockPin,
      byte latchPin,
      int count
    );
    ~ShiftRegisterInput();
    void begin();
    void update();
    bool wasSwitchedOn(int index);
    bool wasSwitchedOff(int index);
    bool getCurrentState(int index);
};

#endif
