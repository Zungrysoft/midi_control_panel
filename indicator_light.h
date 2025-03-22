#ifndef INDICATOR_LIGHT_H
#define INDICATOR_LIGHT_H

#include <Arduino.h>

class IndicatorLight {
  private:
    byte pin;

    bool state;
    float value;
    int brightness;

  public:
    IndicatorLight(
      byte signalPin
    );
    void begin();
    void update();
    void setState(bool state);
    void setValue(float value);
    void setBrightness(int brightness);
};

#endif
