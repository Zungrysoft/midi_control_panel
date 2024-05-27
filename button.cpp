#include "button.h"

Button::Button(
  byte pin
):
  pin(pin)
{};

void Button::begin() {
  pinMode(pin, INPUT_PULLUP);
}

bool Button::wasPressed() {
  bool pressed = !digitalRead(pin);

  // Switched on
  if (pressed && !prevPressed) {
    return true;
  }

  prevPressed = pressed;

  return false;
};
