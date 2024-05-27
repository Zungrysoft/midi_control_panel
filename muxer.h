#ifndef MUXER_H
#define MUXER_H

#include <Arduino.h>

class Muxer {
  private:
    byte analogPin;
    byte pin0;
    byte pin1;
    byte pin2;

  public:
    Muxer(
      byte analogPin,
      byte pin0,
      byte pin1,
      byte pin2
    );
    void begin();
    int read(byte channel);
};

#endif
