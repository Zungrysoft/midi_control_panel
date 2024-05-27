#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

const int DEBOUNCE_TIME_MS = 5;

class Button {
  private:
    byte pin;

    bool prevPressed = 0;

  public:
    Button(
      byte pin
    );
    void begin();
    bool wasPressed();
};

#endif
