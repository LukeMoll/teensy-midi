#include <Bounce.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


#define MIDI_CHAN 1
#define MIDI_CC 5
#define DEBOUNCE_MS 5

int led = 13;
int buttonPin0 = 30;
int buttonPin1 = 31;

Bounce button0 = Bounce(buttonPin0, DEBOUNCE_MS);
Bounce button1 = Bounce(buttonPin1, DEBOUNCE_MS);

bool b0held = false;
bool b1held = false;

uint8_t cc = 0;

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void update_matrix();
void set_pixel(uint8_t idx);
void myControlChange(byte channel, byte control, byte value) {
  Serial.print(channel, DEC);
  Serial.print(" ");
  Serial.print(control, DEC);
  Serial.print(" ");
  Serial.println(value, DEC);
  if(channel == MIDI_CHAN && control == MIDI_CC) {
    cc = value;
    update_matrix();
  }
}

void setup() {
//  Serial.begin(9600);/;
  pinMode(led, OUTPUT);
  pinMode(buttonPin0, INPUT_PULLUP); 
  pinMode(buttonPin1, INPUT_PULLUP); 

  matrix.begin(0x70);
  matrix.setBrightness(1);
  matrix.clear();
//  matrix.drawPixel(0,0, LED_ON);
//  matrix.drawPixel(7,0, LED_ON);
//  matrix.drawPixel(0,7, LED_ON);
  usbMIDI.setHandleControlChange(myControlChange);
  update_matrix();
}

void loop() {
  if(button0.update()) {
    b0held = button0.risingEdge() ? true : false;
//    cc += 16;
//    if(cc > 127) {cc = 127;}
//    usbMIDI.sendControlChange(MIDI_CC, cc, MIDI_CHAN);
  }
  if(button1.update()) {
    b1held = button1.risingEdge() ? true : false;
//    cc -= 16;/
//    if(cc > 127)/ {cc = 0;}
//    usbMIDI.sendCon/trolChange(MIDI_CC, cc, MIDI_CHAN);
  }
  if(b0held ^ b1held) {
    if(b0held) { cc = cc < 127 ? cc + 1 : 127 ;}
    else { cc = cc > 0 ? cc - 1 : 0; }
    usbMIDI.sendControlChange(MIDI_CC, cc, MIDI_CHAN);
    update_matrix();
  }
  delay(10);
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}

void update_matrix() {
  matrix.clear();
  for(uint8_t i=0; i < (cc >> 1); i++) {
//    matrix.drawPixel(i >> 3, i & 7, LED_ON)/;
     set_pixel(i);
  }
  matrix.writeDisplay();
}

void set_pixel(uint8_t idx) {
  if(idx < 64) {
    matrix.drawPixel(idx % 8, idx / 8, LED_ON);
  }
}
