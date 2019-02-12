#include <Bounce.h>

#define MIDI_CHAN 1 // desu

Bounce myButton = Bounce(30, 10);
void setup() {

}

void loop() {
  if(myButton.update()) {
    if(myButton.risingEdge()) {
      usbMIDI.sendNoteOn(60, 99, MIDI_CHAN);
    }
    else if(myButton.fallingEdge()) {
      usbMIDI.sendNoteOff(60, 99, MIDI_CHAN);
    }
  }
  delay(10);
}
