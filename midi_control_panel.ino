// =======================
// Imports and Definitions
// =======================
#include <MIDI.h>
#include "control_set.h"
#include "control.h"
#include "config_laptop.h"
#include "config_synth.h"
#include "indicator_light.h"
#include "switch.h"

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#define BLOCK_CC_PASSTHROUGH true

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI_IN_1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI_IN_2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI_IN_3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI_OUT);

void(* reset) (void) = 0;

// =============
// Configuration
// =============

const int OCTAVE_SWITCH_RANGE = 3;

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

ControlSet controls;

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

  // Set control scheme
  OperationModeSwitch.begin();
  OperationModeSwitch.update();
  bool operationMode = OperationModeSwitch.getCurrentState();
  controls = operationMode ? getControlsForLaptop() : getControlsForSynth();

  // Init shift registers
  SHIFT_REGISTER_INPUT->begin();
  SHIFT_REGISTER_OUTPUT->begin();

  // Init muxers
  for (int i = 0; i < MUXERS_LEN; i ++) {
    MUXERS[i]->begin();
  }

  // Init MIDI controls
  controls.begin();

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
  LightBrightnessSwitch.begin();

  OctaveUpChannel1Button.begin();
  OctaveDownChannel1Button.begin();
  OctaveUpChannel2Button.begin();
  OctaveDownChannel2Button.begin();

  noteIndicator.begin();
  transposeIndicator.begin();
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
  controls.update(MIDI_OUT);
  controls.setBrightness(lightBrightness);

  // Operation Mode switch
  OperationModeSwitch.update();
  if (OperationModeSwitch.wasSwitchedOn() || OperationModeSwitch.wasSwitchedOff()) {
    reset();
  }

  // Control Sync button
  ControlSyncButton.update();
  if (ControlSyncButton.wasSwitchedOn()) {
    controls.forceUpdate(MIDI_OUT);
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
