#ifndef CONFIG_LAPTOP_H
#define CONFIG_LAPTOP_H

#include "config.h"

#include "control_set.h"
#include "control.h"
#include "control_switch.h"
#include "control_momentary_switch.h"
#include "control_leslie_switch.h"
#include "control_pot.h"
#include "control_dial.h"
#include "control_selector.h"

// Declare dial input and output arrays
int DIAL_0_INPUT_VALUES[] = {0, 90, 336, 507, 844, 1023};
int DIAL_0_OUTPUT_VALUES[] = {127, 102, 76, 51, 25, 0};

// Declare selector pin arrays
int SELECTOR_0_INPUT_INDICES[] = {20, 21, 16, 17, 12, 13, 8, 9, 7, 6};
int SELECTOR_0_OUTPUT_INDICES[] = {21, 23, 8, 11, 14, 15, 0, 3, 6, 7};
int SELECTOR_0_CC_NUMBERS[] = {66, 67, 68, 69, 70, 71, 72, 73, 74, 75};
int SELECTOR_0_CHANNELS[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int SELECTOR_1_INPUT_INDICES[] = {23, 22, 18, 19, 14, 15, 10, 11, 4, 5};
int SELECTOR_1_OUTPUT_INDICES[] = {20, 22, 10, 9, 12, 13, 1, 2, 4, 5};
int SELECTOR_1_CC_NUMBERS[] = {76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
int SELECTOR_1_CHANNELS[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

ControlSet getControlsForLaptop() {
  const ControlSet CONTROLS;

  // Toggle Switches Top Row
  CONTROLS.add(new ControlSwitch(27, 63, 1, 0, 127)); // Up
  CONTROLS.add(new ControlSwitch(25, 61, 1, 0, 127)); // Down
  // CONTROLS.add(new ControlSwitch(29, 66, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(31, 66, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(33, 68, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(35, 70, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(37, 72, 1, 0, 127));

  // Toggle Switches Bottom Row
  CONTROLS.add(new ControlSwitch(26, 62, 1, 0, 127)); // Up
  CONTROLS.add(new ControlSwitch(24, 60, 1, 0, 127)); // Down
  CONTROLS.add(new ControlSwitch(28, 64, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(30, 65, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(32, 67, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(34, 69, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(36, 71, 1, 0, 127));

  // Rocker Switches
  CONTROLS.add(new ControlSwitch(23, 66, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(22, 70, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(21, 71, 1, 0, 127));
  CONTROLS.add(new ControlSwitch(20, 72, 1, 0, 127));

  // Foot Switches
  CONTROLS.add(new ControlLeslieSwitch(47, 29, 2, 66, 1, 0, 63, 127));
  CONTROLS.add(new ControlMomentarySwitch(48, 5, 72, 1, 0, 127));

  // Misc. Potentiometers
  CONTROLS.add(new ControlPot(7, 37, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(4, 34, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 6, 22, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(6, 36, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(5, 35, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 5, 13, 1, 0, 127, 0, 1024));

  // Upper Drawbars
  CONTROLS.add(new ControlPot(MUXERS[2], 4, 20, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 3, 19, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 2, 18, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 5, 21, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 3, 11, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 7, 15, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 7, 23, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 0, 16, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[2], 1, 17, 1, 0, 127, 0, 1024));

  // Pedal Drawbars
  CONTROLS.add(new ControlPot(MUXERS[1], 1, 9, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 4, 12, 1, 0, 127, 0, 1024));

  // Lower Drawbars
  CONTROLS.add(new ControlPot(MUXERS[1], 2, 10, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 0, 8, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[1], 6, 14, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 1, 1, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 5, 5, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 2, 2, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 4, 4, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 3, 3, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 7, 7, 1, 0, 127, 0, 1024));

  // Volume Control Pots
  CONTROLS.add(new ControlPot(MUXERS[0], 0, 0, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[0], 6, 6, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 7, 31, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 3, 27, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 1, 25, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 5, 29, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 4, 28, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 2, 26, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 0, 24, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(MUXERS[3], 6, 30, 1, 0, 127, 0, 1024));

  // Expression Pedals
  CONTROLS.add(new ControlPot(8, 7, 1, 0, 127, 0, 1024));
  CONTROLS.add(new ControlPot(9, 8, 1, 0, 127, 0, 1024));

  // Dials
  CONTROLS.add(new ControlDial(10, 73, 1, 6, DIAL_0_INPUT_VALUES, DIAL_0_OUTPUT_VALUES));

  // Selectors
  CONTROLS.add(new ControlSelector(
    SHIFT_REGISTER_INPUT,
    SHIFT_REGISTER_OUTPUT,
    10,
    SELECTOR_0_INPUT_INDICES,
    SELECTOR_0_OUTPUT_INDICES,
    SELECTOR_0_CC_NUMBERS,
    SELECTOR_0_CHANNELS
  ));
  CONTROLS.add(new ControlSelector(
    SHIFT_REGISTER_INPUT,
    SHIFT_REGISTER_OUTPUT,
    10,
    SELECTOR_1_INPUT_INDICES,
    SELECTOR_1_OUTPUT_INDICES,
    SELECTOR_1_CC_NUMBERS,
    SELECTOR_1_CHANNELS
  ));

  return CONTROLS;
};

#endif
