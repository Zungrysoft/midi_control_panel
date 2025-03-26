#ifndef CONFIG_SYNTH_H
#define CONFIG_SYNTH_H

#include "config.h"

#include "control_set.h"
#include "control.h"
#include "control_switch.h"
#include "control_momentary_switch.h"
#include "control_leslie_switch.h"
#include "control_pot.h"
#include "control_dial.h"
#include "control_selector.h"

ControlSet getControlsForSynth() {
  const ControlSet CONTROLS;

  // Toggle Switches Top Row
  CONTROLS.add(new ControlSwitch(27, 63, 1, 0, 127)); // Up
  CONTROLS.add(new ControlSwitch(25, 61, 1, 0, 127)); // Down

  return CONTROLS;
};

#endif
