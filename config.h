#ifndef CONFIG_H
#define CONFIG_H

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#include "config.h"

#include "muxer.h"
#include "shift_register_input.h"
#include "shift_register_output.h"

// Configure muxers
const byte MUXER_SWITCH_A = 49;
const byte MUXER_SWITCH_B = 50;
const byte MUXER_SWITCH_C = 51;
const Muxer* MUXERS[] = {
  new Muxer(0, MUXER_SWITCH_A, MUXER_SWITCH_B, MUXER_SWITCH_C),
  new Muxer(1, MUXER_SWITCH_A, MUXER_SWITCH_B, MUXER_SWITCH_C),
  new Muxer(2, MUXER_SWITCH_A, MUXER_SWITCH_B, MUXER_SWITCH_C),
  new Muxer(3, MUXER_SWITCH_A, MUXER_SWITCH_B, MUXER_SWITCH_C),
};
const unsigned int MUXERS_LEN = ARRAY_SIZE(MUXERS);

const ShiftRegisterInput* SHIFT_REGISTER_INPUT =
  new ShiftRegisterInput(6, 53, 52, 3);

const ShiftRegisterOutput* SHIFT_REGISTER_OUTPUT =
  new ShiftRegisterOutput(10, 12, 11, 13, 4);

#endif
