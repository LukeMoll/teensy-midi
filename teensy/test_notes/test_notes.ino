#include <Bounce.h>

#define MIDI_CHAN 1 // desu
#define DEBOUNCE_MS 5

int led = 13;
int buttonPin0 = 30;
int buttonPin1 = 31;
uint8_t notes = 0;

Bounce button0 = Bounce(buttonPin0, DEBOUNCE_MS);
Bounce button1 = Bounce(buttonPin1, DEBOUNCE_MS);
void setup() {
  pinMode(led, OUTPUT);
  pinMode(buttonPin0, INPUT_PULLUP); 
  pinMode(buttonPin1, INPUT_PULLUP); 
}

void loop() {
  if(button0.update()) {
    if(button0.fallingEdge()) {
      usbMIDI.sendNoteOn(60, 99, MIDI_CHAN);
      notes++;
    }
    else if(button0.risingEdge()) {
      usbMIDI.sendNoteOff(60, 99, MIDI_CHAN);
      notes = notes ? notes-1 : 0;
    }
  }
  if(button1.update()) {
    if(button1.fallingEdge()) {
      usbMIDI.sendNoteOn(61, 99, MIDI_CHAN);
      notes++;
    }
    else if(button1.risingEdge()) {
      usbMIDI.sendNoteOff(61, 99, MIDI_CHAN);
      notes = notes > 0 ? notes - 1 : 0;
    }
  }

  digitalWrite(led, notes > 0 ? HIGH : LOW);
  
  delay(10);
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}