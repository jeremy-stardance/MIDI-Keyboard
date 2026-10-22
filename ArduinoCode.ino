#include <MIDIUSB.h>

// Button pins: D13 to D20
const byte buttons[8] = {13, 14, 15, 16, 17, 18, 19, 20};

// MIDI notes assigned to each button
const byte notes[8] = {36, 38, 40, 41, 43, 45, 47, 48};

// Track whether each button is pressed
bool buttonStates[8] = {false};

// Send a MIDI Note On message over USB
void sendNoteOn(byte note, byte velocity, byte channel) {
  midiEventPacket_t event = {
    0x09,
    (byte)(0x90 | (channel - 1)),
    note,
    velocity
  };

  MidiUSB.sendMIDI(event);
}

// Send a MIDI Note Off message over USB
void sendNoteOff(byte note, byte velocity, byte channel) {
  midiEventPacket_t event = {
    0x08,
    (byte)(0x80 | (channel - 1)),
    note,
    velocity
  };

  MidiUSB.sendMIDI(event);
}

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void loop() {
  for (byte i = 0; i < 8; i++) {
    
    bool pressed = (digitalRead(buttons[i]) == LOW);

    if (pressed && !buttonStates[i]) {
      sendNoteOn(notes[i], 127, 1);
      buttonStates[i] = true;
    }

    else if (!pressed && buttonStates[i]) {
      sendNoteOff(notes[i], 0, 1);
      buttonStates[i] = false;
    }
  }

  // Transmit queued MIDI messages over USB
  MidiUSB.flush();
}