#include "switch.h"

Switch::Switch(
  byte pin
):
  pin(pin)
{};

void Switch::begin() {
  pinMode(pin, INPUT_PULLUP);
}

void Switch::update() {
  prevPressed = curPressed;
  curPressed = !digitalRead(pin);
};

bool Switch::wasSwitchedOn() {
  if (curPressed && !prevPressed) {
    return true;
  }
  return false;
}

bool Switch::wasSwitchedOff() {
  if (!curPressed && prevPressed) {
    return true;
  }
  return false;
}

bool Switch::getCurrentState() {
  return curPressed;
}
