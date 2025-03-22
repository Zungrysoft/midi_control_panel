// =======================
// Imports and Definitions
// =======================
#include <MIDI.h>
#include "muxer.h"
#include "shift_register_input.h"
#include "shift_register_output.h"
#include "control.h"
#include "control_switch.h"
#include "control_momentary_switch.h"
#include "control_leslie_switch.h"
#include "control_pot.h"
#include "control_dial.h"
#include "control_selector.h"
#include "indicator_light.h"
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

const int OCTAVE_SWITCH_RANGE = 3;

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

const ShiftRegisterInput* SHIFT_REGISTER_INPUT =
  new ShiftRegisterInput(6, 53, 52, 3);

const ShiftRegisterOutput* SHIFT_REGISTER_OUTPUT =
  new ShiftRegisterOutput(10, 12, 11, 13, 4);

// Declare dial input and output arrays
int DIAL_0_INPUT_VALUES[] = {0, 175, 512, 680, 930, 1024};
int DIAL_0_OUTPUT_VALUES[] = {0, 25, 51, 76, 102, 127};

// Declare selector pin arrays
int SELECTOR_0_INPUT_INDICES[] = {20, 21, 16, 17, 12, 13, 8, 9, 7, 6};
int SELECTOR_0_OUTPUT_INDICES[] = {21, 23, 8, 11, 14, 15, 0, 3, 6, 7};
int SELECTOR_0_CC_NUMBERS[] = {66, 67, 68, 69, 70, 71, 72, 73, 74, 75};
int SELECTOR_0_CHANNELS[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int SELECTOR_1_INPUT_INDICES[] = {23, 22, 18, 19, 14, 15, 10, 11, 4, 5};
int SELECTOR_1_OUTPUT_INDICES[] = {20, 22, 10, 9, 12, 13, 1, 2, 4, 5};
int SELECTOR_1_CC_NUMBERS[] = {76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
int SELECTOR_1_CHANNELS[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Configure MIDI controls
const Control* CONTROLS[] = {

  // 3-Position Toggle Switches
  new ControlSwitch(24, 66, 1, 0, 127),
  new ControlSwitch(25, 66, 1, 0, 127),
  new ControlSwitch(26, 66, 1, 0, 127),
  new ControlSwitch(27, 66, 1, 0, 127),

  // Toggle Switches
  new ControlSwitch(28, 66, 1, 0, 127),
  // new ControlSwitch(29, 66, 1, 0, 127),
  new ControlSwitch(30, 66, 1, 0, 127),
  new ControlSwitch(31, 66, 1, 0, 127),
  new ControlSwitch(32, 66, 1, 0, 127),
  new ControlSwitch(33, 66, 1, 0, 127),
  new ControlSwitch(34, 66, 1, 0, 127),
  new ControlSwitch(35, 66, 1, 0, 127),
  new ControlSwitch(36, 66, 1, 0, 127),
  new ControlSwitch(37, 66, 1, 0, 127),

  // Rocker Switches
  new ControlSwitch(23, 66, 1, 0, 127),
  new ControlSwitch(22, 70, 1, 0, 127),
  new ControlSwitch(21, 71, 1, 0, 127),
  new ControlSwitch(20, 72, 1, 0, 127),

  // Foot Switches
  new ControlLeslieSwitch(47, 29, 2, 66, 1, 0, 63, 127),
  new ControlMomentarySwitch(48, 5, 72, 1, 0, 127),

  // // Potentiometers
  // new ControlPot(4, 66, 1, 0, 127, 0, 1024),
  // new ControlPot(5, 66, 1, 0, 127, 0, 1024),
  // new ControlPot(6, 66, 1, 0, 127, 0, 1024),
  // new ControlPot(7, 66, 1, 0, 127, 0, 1024),

  // // Drawbars

  // // Expression Pedals
  // new ControlPot(8, 7, 1, 0, 127, 0, 1024),
  // new ControlPot(9, 7, 1, 0, 127, 0, 1024),

  // Dials
  new ControlDial(10, 73, 1, 6, DIAL_0_INPUT_VALUES, DIAL_0_OUTPUT_VALUES),

  // Selectors
  new ControlSelector(
    SHIFT_REGISTER_INPUT,
    SHIFT_REGISTER_OUTPUT,
    10,
    SELECTOR_0_INPUT_INDICES,
    SELECTOR_0_OUTPUT_INDICES,
    SELECTOR_0_CC_NUMBERS,
    SELECTOR_0_CHANNELS
  ),
  new ControlSelector(
    SHIFT_REGISTER_INPUT,
    SHIFT_REGISTER_OUTPUT,
    10,
    SELECTOR_1_INPUT_INDICES,
    SELECTOR_1_OUTPUT_INDICES,
    SELECTOR_1_CC_NUMBERS,
    SELECTOR_1_CHANNELS
  ),
  
  // new ControlPot(MUXERS[0], 0, 0, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 1, 1, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 2, 2, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 3, 3, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 4, 4, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 5, 5, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 6, 6, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[0], 7, 7, 1, 0, 127, 0, 1024),
  
  // new ControlPot(MUXERS[1], 0, 8, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 1, 9, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 2, 10, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 3, 11, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 4, 12, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 5, 13, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 6, 14, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[1], 7, 15, 1, 0, 127, 0, 1024),
  
  // new ControlPot(MUXERS[2], 0, 16, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 1, 17, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 2, 18, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 3, 19, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 4, 20, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 5, 21, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 6, 22, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[2], 7, 23, 1, 0, 127, 0, 1024),
  
  // new ControlPot(MUXERS[3], 0, 24, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 1, 25, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 2, 26, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 3, 27, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 4, 28, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 5, 29, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 6, 30, 1, 0, 127, 0, 1024),
  // new ControlPot(MUXERS[3], 7, 31, 1, 0, 127, 0, 1024),
};

// Configure special switches
const Switch ControlSyncButton = Switch(45);
const Switch PanicButton = Switch(44);
const Switch TransposeButton = Switch(43);
const Switch OperationModeSwitch = Switch(46);
const Switch LightBrightnessSwitch = Switch(42);

const Switch OctaveUpChannel1Button = Switch(38);
const Switch OctaveDownChannel1Button = Switch(39);
const Switch OctaveUpChannel2Button = Switch(40);
const Switch OctaveDownChannel2Button = Switch(41);

const byte octaveUpChannel1LightIndices[] = {19, 29, 28};
const byte octaveDownChannel1LightIndices[] = {27, 24, 17};
const byte octaveUpChannel2LightIndices[] = {31, 30, 18};
const byte octaveDownChannel2LightIndices[] = {25, 26, 16};

const IndicatorLight noteIndicator = IndicatorLight(4);

const byte lightBrightnessHigh = 255;
const byte lightBrightnessLow = 10;

// =========
// Main Code
// =========

short int octaveChannel1 = 0;
short int octaveChannel2 = 0;

long unsigned int lastControlChange = 0;

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

  // Init shift registers
  SHIFT_REGISTER_INPUT->begin();
  SHIFT_REGISTER_OUTPUT->begin();
  // SHIFT_REGISTER_OUTPUT->setBrightness(2);

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
  TransposeButton.begin();
  OperationModeSwitch.begin();
  LightBrightnessSwitch.begin();

  OctaveUpChannel1Button.begin();
  OctaveDownChannel1Button.begin();
  OctaveUpChannel2Button.begin();
  OctaveDownChannel2Button.begin();

  noteIndicator.begin();
}

void loop() {
  // MIDI Passthrough
  MIDI_IN_1.read();
  MIDI_IN_2.read();
  MIDI_IN_3.read();

  // Light brightness switch
  LightBrightnessSwitch.update();
  int lightBrightness = LightBrightnessSwitch.getCurrentState() ? lightBrightnessHigh : lightBrightnessLow;

  // Update input shift register
  SHIFT_REGISTER_INPUT->update();

  // Update MIDI controls
  for (int i = 0; i < ARRAY_SIZE(CONTROLS); i ++) {
    CONTROLS[i]->update(MIDI_OUT);
    CONTROLS[i]->setBrightness(lightBrightness);
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

  // Octave settings
  OctaveUpChannel1Button.update();
  OctaveDownChannel1Button.update();
  OctaveUpChannel2Button.update();
  OctaveDownChannel2Button.update();
  if (OctaveUpChannel1Button.wasSwitchedOn() && octaveChannel1 < OCTAVE_SWITCH_RANGE) {
    octaveChannel1 ++;

    // Iterate over all currently on notes, save them,
  }
  if (OctaveDownChannel1Button.wasSwitchedOn() && octaveChannel1 > -OCTAVE_SWITCH_RANGE) {
    octaveChannel1 --;

    // Iterate over all currently on notes, save them,
  }
  if (OctaveUpChannel2Button.wasSwitchedOn() && octaveChannel2 < OCTAVE_SWITCH_RANGE) {
    octaveChannel2 ++;

    // Iterate over all currently on notes, save them,
  }
  if (OctaveDownChannel2Button.wasSwitchedOn() && octaveChannel2 > -OCTAVE_SWITCH_RANGE) {
    octaveChannel2 --;

    // Iterate over all currently on notes, save them,
  }

  // Update Octave indicator lights
  for (int i = 0; i < OCTAVE_SWITCH_RANGE; i ++) {
    SHIFT_REGISTER_OUTPUT->write(octaveUpChannel1LightIndices[i], octaveChannel1 > i);
    SHIFT_REGISTER_OUTPUT->write(octaveDownChannel1LightIndices[i], octaveChannel1 < -i);
    SHIFT_REGISTER_OUTPUT->write(octaveUpChannel2LightIndices[i], octaveChannel2 > i);
    SHIFT_REGISTER_OUTPUT->write(octaveDownChannel2LightIndices[i], octaveChannel2 < -i);
  }

  // Lights
  noteIndicator.setState(millis() - lastControlChange < 10);
  noteIndicator.update();
  SHIFT_REGISTER_OUTPUT->update();
  SHIFT_REGISTER_OUTPUT->setBrightness(lightBrightness);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  lastControlChange = millis();
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
