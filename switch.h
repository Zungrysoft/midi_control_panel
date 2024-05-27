#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>

const byte DEBOUNCE_TIME_MS = 5;

class Switch {
  private:
    byte pin;
    
    bool curPressed = false;
    bool prevPressed = false;

  public:
    Switch(
      byte pin
    );
    void begin();
    void update();
    bool wasSwitchedOn();
    bool wasSwitchedOff();
    bool getCurrentState();
};

#endif
