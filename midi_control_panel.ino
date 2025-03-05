// =======================
// Imports and Definitions
// =======================
#include <MIDI.h>
#include "muxer.h"
#include "control.h"
#include "control_switch.h"
#include "control_pot.h"
#include "control_dial.h"
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

#define BLOCK_CC_PASSTHROUGH true

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI_IN_1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI_IN_2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI_IN_3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI_OUT);

// =============
// Configuration
// =============

// Configure muxers
const Muxer* MUXERS[] = {
  new Muxer(0, 48, 49, 50),
  new Muxer(0, 30, 31, 32),
};

// Declare dial input and output arrays
int DIAL_0_INPUT_VALUES[] = {0, 175, 512, 680, 930, 1024};
int DIAL_0_OUTPUT_VALUES[] = {0, 25, 51, 76, 102, 127};

// Declare selector pin arrays
int SELECTOR_0_SWITCH_PINS[] = {45, 46, 47};
int SELECTOR_0_LIGHT_PINS[] = {48, 49, 50};

// Configure MIDI controls
const Control* CONTROLS[] = {
  // new ControlSwitch(38, 66, 1, 0, 127),
  // new ControlSwitch(39, 70, 1, 0, 127),
  // new ControlPot(MUXERS[0], MUX0, 66, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], MUX1, 66, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], MUX0, 67, 1, 0, 127, 0, 1024),
  new ControlPot(10, 1, 1, 0, 127, 0, 1024),
  // new ControlDial(3, 66, 1, 6, DIAL_0_INPUT_VALUES, DIAL_0_OUTPUT_VALUES),
};

// Configure special switches
const Switch ControlSyncButton = Switch(40);
const Switch PanicButton = Switch(41);
const Switch TransposeButton = Switch(58);
const Switch ControlPassthroughSwitch = Switch(53);

const Switch OctaveUpUpper = Switch(54);
const Switch OctaveDownUpper = Switch(55);
const Switch OctaveUpLower = Switch(56);
const Switch OctaveDownLower = Switch(57);

// =========
// Main Code
// =========

void setup() {
  // MIDI setup
  Serial.begin(31250);

  MIDI_IN_1.begin(MIDI_CHANNEL_OMNI);
  MIDI_IN_2.begin(MIDI_CHANNEL_OMNI);
  MIDI_IN_3.begin(MIDI_CHANNEL_OMNI);
  MIDI_OUT.begin(MIDI_CHANNEL_OMNI);

  MIDI_IN_1.setHandleNoteOn(handleNoteOn);
  MIDI_IN_1.setHandleNoteOff(handleNoteOff);
  MIDI_IN_1.setHandleControlChange(handleControlChange);
  MIDI_IN_1.setHandlePitchBend(handlePitchBend);
  MIDI_IN_1.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI_IN_1.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI_IN_1.setHandleProgramChange(handleProgramChange);

  MIDI_IN_2.setHandleNoteOn(handleNoteOn);
  MIDI_IN_2.setHandleNoteOff(handleNoteOff);
  MIDI_IN_2.setHandleControlChange(handleControlChange);
  MIDI_IN_2.setHandlePitchBend(handlePitchBend);
  MIDI_IN_2.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI_IN_2.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI_IN_2.setHandleProgramChange(handleProgramChange);

  MIDI_IN_3.setHandleNoteOn(handleNoteOn);
  MIDI_IN_3.setHandleNoteOff(handleNoteOff);
  MIDI_IN_3.setHandleControlChange(handleControlChange);
  MIDI_IN_3.setHandlePitchBend(handlePitchBend);
  MIDI_IN_3.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI_IN_3.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI_IN_3.setHandleProgramChange(handleProgramChange);

  // Init muxers
  for (int i = 0; i < ARRAY_SIZE(MUXERS); i ++) {
    MUXERS[i]->begin();
  }

  // Init MIDI controls
  for (int i = 0; i < ARRAY_SIZE(CONTROLS); i ++) {
    CONTROLS[i]->begin();
  }

  // Special controls
  ControlSyncButton.begin();
  PanicButton.begin();
  ControlPassthroughSwitch.begin();
}

void loop() {
  // MIDI Passthrough
  MIDI_IN_1.read();
  MIDI_IN_2.read();
  MIDI_IN_3.read();

  // Update MIDI controls
  for (int i = 0; i < ARRAY_SIZE(CONTROLS); i ++) {
    CONTROLS[i]->update(MIDI_OUT);
  }

  // Control Sync button
  ControlSyncButton.update();
  if (ControlSyncButton.wasSwitchedOn()) {
    for (int i = 0; i < ARRAY_SIZE(CONTROLS); i ++) {
      CONTROLS[i]->forceUpdate(MIDI_OUT);
    }
  }

  // Panic button
  PanicButton.update();
  if (PanicButton.wasSwitchedOn()) {
    for (int i = 1; i <= 16; i ++) {
      MIDI_OUT.sendControlChange(123, 0, i);
    }
  }

  // Control Passthrough Switch
  ControlPassthroughSwitch.update();
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  MIDI_OUT.sendNoteOn(note, velocity, channel);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  MIDI_OUT.sendNoteOff(note, velocity, channel);
}

void handleControlChange(byte channel, byte control, byte value) {
  // Make sure control passthrough isn't blocked
  if (!BLOCK_CC_PASSTHROUGH) {
    MIDI_OUT.sendControlChange(control, value, channel);
  }
}

void handlePitchBend(byte channel, int bend) {
  // Make sure control passthrough isn't blocked
  if (!BLOCK_CC_PASSTHROUGH) {
    MIDI_OUT.sendPitchBend(bend, channel);
  }
}

void handleAfterTouchChannel(byte channel, byte pressure) {
  MIDI_OUT.sendAfterTouch(pressure, channel);
}

void handleAfterTouchPoly(byte channel, byte note, byte pressure) {
  MIDI_OUT.sendPolyPressure(note, pressure, channel);
}

void handleProgramChange(byte channel, byte program) {
  // Make sure control passthrough isn't blocked
  if (!BLOCK_CC_PASSTHROUGH) {
    MIDI_OUT.sendProgramChange(program, channel);
  }
}
