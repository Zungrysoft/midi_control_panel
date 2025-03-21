#ifndef SHIFT_REGISTER_OUTPUT_H
#define SHIFT_REGISTER_OUTPUT_H

#include <Arduino.h>

class ShiftRegisterOutput {
  private:
    byte signalPin;
    byte clockPin;
    byte latchPin;
    byte outputEnablePin;
    int count;

    int brightness;
    byte* data;

  public:
    ShiftRegisterOutput(
      byte signalPin,
      byte clockPin,
      byte latchPin,
      byte outputEnablePin,
      int count
    );
    ~ShiftRegisterOutput();
    void begin();
    void update();
    void write(int index, bool state);
    void setBrightness(int brightness);
};

#endif
