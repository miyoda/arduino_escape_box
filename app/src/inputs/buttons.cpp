#include "protothreads.h"
#include "Arduino.h"

int const PIN_BUTTON1 = 53;

int getButton1() {
  digitalRead(PIN_BUTTON1);
}

void setup_buttons() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP); 
}

void loop_buttons() {

}
