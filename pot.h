#ifndef POT_H
#define POT_H

#include <Arduino.h>
#include "muxer.h"

const int DEFUZZ_BUFFER = 5;

class Pot {
  private:
    Muxer* muxer;
    byte pin;

    int curValue = 0;
    bool valueChanged = true;

  public:
    Pot(
      Muxer* muxer,
      byte pin
    );
    Pot(
      byte pin
    );
    void update();
    bool wasValueChanged();
    int getCurrentValue();
};

#endif
