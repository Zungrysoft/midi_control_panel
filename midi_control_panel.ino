// =======================
// Imports and Definitions
// =======================
#include <MIDI.h>
#include "control_switch.h"
#include "muxer.h"
#include "control_pot.h"
#include "switch.h"

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#define MUX0 4
#define MUX1 6
#define MUX2 7
#define MUX3 5
#define MUX4 2
#define MUX5 1
#define MUX6 0
#define MUX7 3

MIDI_CREATE_DEFAULT_INSTANCE();

// =============
// Configuration
// =============

// Configure switches
const ControlSwitch SWITCHES[] = {
  ControlSwitch(38, 66, 1, 0, 127),
  ControlSwitch(39, 70, 1, 0, 127),
};

// Configure muxers
const Muxer MUXERS[] = {
  Muxer(0, 48, 49, 50),
};

// Configure pots
const ControlPot POTS[] = {
  ControlPot(MUXERS[0], MUX0, 66, 1, 0, 127, 0, 1024),
};

// Configure special switches
const Switch ControlSyncButton = Switch(40);
const Switch PanicButton = Switch(41);
const Switch ControlPassthroughSwitch = Switch(53);

// =========
// Main Code
// =========

// Tracks states of switches between loops
bool switchPrevStates[ARRAY_SIZE(SWITCHES)];
int potPrevReadValues[ARRAY_SIZE(POTS)];
int potPrevValues[ARRAY_SIZE(POTS)];

void setup() {
  // MIDI setup
  Serial.begin(31250);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // MIDI passthrough functions
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI.setHandleProgramChange(handleProgramChange);

  // Init switches
  for (int i = 0; i < ARRAY_SIZE(SWITCHES); i ++) {
    SWITCHES[i].begin();
  }

  // Init muxers
  for (int i = 0; i < ARRAY_SIZE(MUXERS); i ++) {
    MUXERS[i].begin();
  }

  // Special switches
  ControlSyncButton.begin();
  PanicButton.begin();
  ControlPassthroughSwitch.begin();
}

void loop() {
  // MIDI Passthrough
  MIDI.read();

  // Switches
  for (int i = 0; i < ARRAY_SIZE(SWITCHES); i ++) {
    SWITCHES[i].update(MIDI);
  }

  // Pots
  for (int i = 0; i < ARRAY_SIZE(POTS); i ++) {
    POTS[i].update(MIDI);
  }

  // Control Sync button
  ControlSyncButton.update();
  if (ControlSyncButton.wasSwitchedOn()) {
    // Switches
    for (int i = 0; i < ARRAY_SIZE(SWITCHES); i ++) {
      SWITCHES[i].forceUpdate(MIDI);
    }

    // Pots
    for (int i = 0; i < ARRAY_SIZE(POTS); i ++) {
      POTS[i].forceUpdate(MIDI);
    }
  }

  // Panic button
  PanicButton.update();
  if (PanicButton.wasSwitchedOn()) {
    for (int i = 1; i <= 16; i ++) {
      MIDI.sendControlChange(123, 0, i);
    }
  }

  // Control Passthrough Switch
  ControlPassthroughSwitch.update();
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOn(note, velocity, channel);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOff(note, velocity, channel);
}

void handleControlChange(byte channel, byte control, byte value) {
  // Make sure control passthrough isn't blocked
  if (!ControlPassthroughSwitch.getCurrentState()) {
    MIDI.sendControlChange(control, value, channel);
  }
}

void handlePitchBend(byte channel, int bend) {
  // Make sure control passthrough isn't blocked
  if (!ControlPassthroughSwitch.getCurrentState()) {
    MIDI.sendPitchBend(bend, channel);
  }
}

void handleAfterTouchChannel(byte channel, byte pressure) {
  MIDI.sendAfterTouch(pressure, channel);
}

void handleAfterTouchPoly(byte channel, byte note, byte pressure) {
  MIDI.sendPolyPressure(note, pressure, channel);
}

void handleProgramChange(byte channel, byte program) {
  // Make sure control passthrough isn't blocked
  if (!ControlPassthroughSwitch.getCurrentState()) {
    MIDI.sendProgramChange(program, channel);
  }
}
