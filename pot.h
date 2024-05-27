#ifndef POT_H
#define POT_H

#include <Arduino.h>
#include "muxer.h"
#include "helpers.h"

const int DEFUZZ_BUFFER = 5;

class Pot {
  private:
    Muxer muxer;
    byte muxerChannel;

    int curValue = 0;
    bool valueChanged = true;

  public:
    Pot(
      Muxer muxer,
      byte muxerChannel
    );
    void update();
    bool wasValueChanged();
    int getCurrentValue();
};

#endif
