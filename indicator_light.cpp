#include "indicator_light.h"

IndicatorLight::IndicatorLight(
  byte pin
):
  pin(pin),
  brightness(255),
  value(1.0),
  state(false)
{};

void IndicatorLight::begin() {
  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
};

void IndicatorLight::update() {
  if (state) {
    analogWrite(pin, brightness * value);
  }
  else {
    analogWrite(pin, 0);
  }
};

void IndicatorLight::setState(bool state) {
  this->state = state;
};

void IndicatorLight::setValue(float value) {
  this->value = constrain(value, 0.0, 1.0);
};

void IndicatorLight::setBrightness(int brightness) {
  this->brightness = brightness;
};
