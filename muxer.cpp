#include "muxer.h"

Muxer::Muxer(
  byte analogPin,
  byte pin0,
  byte pin1,
  byte pin2
):
  analogPin(analogPin),
  pin0(pin0),
  pin1(pin1),
  pin2(pin2)
{};

void Muxer::begin() {
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
};

int Muxer::read(byte channel) {
  delay(10);
  digitalWrite(pin0, channel & 1<<0);
  digitalWrite(pin1, channel & 1<<1);
  digitalWrite(pin2, channel & 1<<2);
  delay(10);
  return analogRead(analogPin);
};
