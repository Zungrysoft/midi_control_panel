#ifndef POT_H
#define POT_H

#include <Arduino.h>
#include "muxer.h"

const int DEFUZZ_BUFFER = 5;
const int TIME_AVERAGING_WINDOW = 10;

class Pot {
  private:
    Muxer* muxer;
    byte pin;

    int curValue = 0;
    int readValues[TIME_AVERAGING_WINDOW];
    bool valueChanged = true;

    int readFromPin();

  public:
    Pot(
      Muxer* muxer,
      byte pin
    );
    Pot(
      byte pin
    );
    void begin();
    void update();
    bool wasValueChanged();
    int getCurrentValue();
};

#endif
