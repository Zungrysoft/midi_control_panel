#ifndef MUXER_H
#define MUXER_H

#include <Arduino.h>

const int SLEEP_TIME_MS = 10;

class Muxer {
  private:
    byte analogPin;
    byte pin0;
    byte pin1;
    byte pin2;
    bool curChannel = 0;

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
