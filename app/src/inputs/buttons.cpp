#include "protothreads.h"
#include "Arduino.h"

int const PIN_BUTTON1 = 53;

int checkButton1() {
  return digitalRead(PIN_BUTTON1) == LOW;
}

void setup_buttons() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP); 
}

void loop_buttons() {

}
