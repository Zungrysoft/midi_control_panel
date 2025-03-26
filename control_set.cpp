#include "control_set.h"


ControlSetNode::ControlSetNode(
  Control* control
):
  control(control)
{};


void ControlSet::add(Control* control) {
  const ControlSetNode* newNode = new ControlSetNode(control);
  if (head == nullptr) {
    head = newNode;
    tail = newNode;
  }
  else {
    tail->next = newNode;
    tail = newNode;
  }
};

void ControlSet::begin() {
  ControlSetNode* cur = head;
  while (cur != nullptr) {
    cur->control->begin();
    cur = cur->next;
  }
};

void ControlSet::update(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  ControlSetNode* cur = head;
  while (cur != nullptr) {
    cur->control->update(midiInstance);
    cur = cur->next;
  }
};

void ControlSet::forceUpdate(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> midiInstance) {
  ControlSetNode* cur = head;
  while (cur != nullptr) {
    cur->control->forceUpdate(midiInstance);
    cur = cur->next;
  }
}

void ControlSet::setBrightness(int brightness) {
  ControlSetNode* cur = head;
  while (cur != nullptr) {
    cur->control->setBrightness(brightness);
    cur = cur->next;
  }
}
