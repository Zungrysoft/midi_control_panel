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
  new ControlPot(8, 7, 1, 0, 127, 0, 1024),
  new ControlPot(9, 8, 1, 0, 127, 0, 1024),

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
const Switch ControlSyncButton = Switch(44);
const Switch PanicButton = Switch(43);
const Switch TransposeButton = Switch(45);
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

const IndicatorLight noteIndicator = IndicatorLight(3);
const IndicatorLight transposeIndicator = IndicatorLight(4);

const byte lightBrightnessHigh = 255;
const byte lightBrightnessLow = 10;

// =========
// Main Code
// =========

short int octaveChannel1 = 0;
short int octaveChannel2 = 0;
short int transpose = 1;
bool transposeEnabled = false;
bool transposeWaitingForNote = false;
short int transposeSelected = 0;

byte noteVelocitiesChannel1[128];
byte noteVelocitiesChannel2[128];
byte noteVelocitiesChannel3[128];

long unsigned int lastNote = 0;

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

  // Init muxers
  for (int i = 0; i < ARRAY_SIZE(MUXERS); i ++) {
    MUXERS[i]->begin();
  }

  // Init MIDI controls
  for (int i = 0; i < ARRAY_SIZE(CONTROLS); i ++) {
    CONTROLS[i]->begin();
  }

  // Initialize noteVelocities lists
  // 0 indicates note off in the MIDI standard
  for (int i = 0; i < 128; i ++) {
    noteVelocitiesChannel1[i] = 0;
    noteVelocitiesChannel2[i] = 0;
    noteVelocitiesChannel3[i] = 0;
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
    // Send panic signal on all 16 channels
    for (int i = 1; i <= 16; i ++) {
      MIDI_OUT.sendControlChange(123, 0, i);
    }

    // Set all note velocities to 0
    for (int i = 0; i < 128; i ++) {
      noteVelocitiesChannel1[i] = 0;
      noteVelocitiesChannel2[i] = 0;
      noteVelocitiesChannel3[i] = 0;
    }
  }

  // Transpose setting
  TransposeButton.update();
  if (TransposeButton.wasSwitchedOn()) {
    transposeWaitingForNote = true;
    transposeSelected = transpose;
  }
  else if (TransposeButton.wasSwitchedOff()) {
    transposeWaitingForNote = false;
    short int prevTranspose = transposeEnabled ? transpose : 0;
    if (transposeSelected != prevTranspose && transposeSelected != 0) {
      shiftNotes(transposeSelected - prevTranspose);
      transposeEnabled = true;
      transpose = transposeSelected;
    }
    else if (!transposeEnabled && transposeSelected != 0) {
      shiftNotes(transposeSelected);
      transposeEnabled = true;
    }
    else {
      shiftNotes(-transposeSelected);
      transposeEnabled = false;
    }
  }

  // Octave settings
  OctaveUpChannel1Button.update();
  OctaveDownChannel1Button.update();
  OctaveUpChannel2Button.update();
  OctaveDownChannel2Button.update();
  if (OctaveUpChannel1Button.wasSwitchedOn() && octaveChannel1 < OCTAVE_SWITCH_RANGE) {
    octaveChannel1 ++;
    shiftNotes(12, 1);
  }
  if (OctaveDownChannel1Button.wasSwitchedOn() && octaveChannel1 > -OCTAVE_SWITCH_RANGE) {
    octaveChannel1 --;
    shiftNotes(-12, 1);
  }
  if (OctaveUpChannel2Button.wasSwitchedOn() && octaveChannel2 < OCTAVE_SWITCH_RANGE) {
    octaveChannel2 ++;
    shiftNotes(12, 2);
  }
  if (OctaveDownChannel2Button.wasSwitchedOn() && octaveChannel2 > -OCTAVE_SWITCH_RANGE) {
    octaveChannel2 --;
    shiftNotes(-12, 2);
  }

  // Update Octave indicator lights
  for (int i = 0; i < OCTAVE_SWITCH_RANGE; i ++) {
    SHIFT_REGISTER_OUTPUT->write(octaveUpChannel1LightIndices[i], octaveChannel1 > i);
    SHIFT_REGISTER_OUTPUT->write(octaveDownChannel1LightIndices[i], octaveChannel1 < -i);
    SHIFT_REGISTER_OUTPUT->write(octaveUpChannel2LightIndices[i], octaveChannel2 > i);
    SHIFT_REGISTER_OUTPUT->write(octaveDownChannel2LightIndices[i], octaveChannel2 < -i);
  }

  // Lights
  noteIndicator.setState(millis() - lastNote < 10);
  noteIndicator.setBrightness(lightBrightness);
  noteIndicator.update();
  transposeIndicator.setState(transposeEnabled);
  transposeIndicator.setBrightness(lightBrightness);
  transposeIndicator.update();
  SHIFT_REGISTER_OUTPUT->update();
  SHIFT_REGISTER_OUTPUT->setBrightness(lightBrightness);
}

// When Octave or Transpose setting is changed, all active notes need to be shifted
// to match the new settings, triggering Note-On and Note-Off messages as needed
void shiftNotes(short int offset) {
  shiftNotes(offset, 1);
  shiftNotes(offset, 2);
  shiftNotes(offset, 3);
}

void shiftNotes(short int offset, byte channel) {
  // Early exit there is no offset
  if (offset == 0) {
    return;
  }

  // Transpose does not apply to channels > 3
  if (channel > 3) {
    return;
  }

  // Iterate forwards if shifting down and backwards if shifting up
  // to prevent notes from writing over each other in the array
  int i = offset < 0 ? 0 : 128 - 1;
  while ((offset < 0 && i < 128) || (offset > 0 && i >= 0)) {
    int iNew = i + offset;
    byte velocity = getNoteVelocity(i, channel);
    if (velocity > 0) {
      if (iNew == constrain(iNew, 0, 127)) {
        setNoteVelocity(iNew, velocity, channel);
        MIDI_OUT.sendNoteOn(iNew, velocity, channel);
      }
      setNoteVelocity(i, 0, channel);
      MIDI_OUT.sendNoteOff(i, 0, channel);
    }

    i += offset < 0 ? 1 : -1;
  }
}

void setNoteVelocity(byte note, byte velocity, byte channel) {
  if (channel == 1) {
    noteVelocitiesChannel1[note] = velocity;
  }
  else if (channel == 2) {
    noteVelocitiesChannel2[note] = velocity;
  }
  else if (channel == 3) {
    noteVelocitiesChannel3[note] = velocity;
  }
}

byte getNoteVelocity(byte note, byte channel) {
  if (channel == 1) {
    return noteVelocitiesChannel1[note];
  }
  if (channel == 2) {
    return noteVelocitiesChannel2[note];
  }
  if (channel == 3) {
    return noteVelocitiesChannel3[note];
  }
  return 0;
}

bool getAreNotesActive() {
  for (int i = 0; i < 128; i ++) {
    if (noteVelocitiesChannel1[i] > 0) {
      return true;
    }
    if (noteVelocitiesChannel2[i] > 0) {
      return true;
    }
    if (noteVelocitiesChannel3[i] > 0) {
      return true;
    }
  }
  return false;
}

byte transposeNote(byte note, byte channel) {
  byte ret = note;

  if (transposeEnabled && channel <= 3) {
    ret += transpose;
  }

  if (channel == 1) {
    ret += octaveChannel1 * 12;
  }
  else if (channel == 2) {
    ret += octaveChannel2 * 12;
  }

  return ret;
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (transposeWaitingForNote) {
    transposeSelected = note - 60;
    if (transposeSelected >= 0) {
      transposeSelected = transposeSelected % 12;
    }
    else {
      transposeSelected = (transposeSelected % 12) - 12;
      if (transposeSelected == -12) {
        transposeSelected = 0;
      }
    }
  }
  else {
    lastNote = millis();
    byte transposedNote = transposeNote(note, channel);
    if (transposedNote == constrain(transposedNote, 0, 127)) {
      MIDI_OUT.sendNoteOn(transposedNote, velocity, channel);
      setNoteVelocity(transposedNote, velocity, channel);
    }
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  byte transposedNote = transposeNote(note, channel);
  if (transposedNote == constrain(transposedNote, 0, 127)) {
    MIDI_OUT.sendNoteOff(transposedNote, velocity, channel);
    setNoteVelocity(transposedNote, 0, channel);
  }
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
