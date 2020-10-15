#include <protothreads.h>
#include <Arduino.h>
#include "rotatory_encoder.h"

/*+
  Rotatory encoder module.
  https://zaragozamakerspace.com/index.php/introduccion-al-rotary-encoder/
*/

int const PIN_ROTATORY_ENCODER_SW = 5;
int const PIN_ROTATORY_ENCODER_DT = 4;
int const PIN_ROTATORY_ENCODER_CLK = 3;

static int lastRotatoryEncoderValue, rotatoryEncoderPos = 0;

void isr() {
  int newDt = digitalRead(PIN_ROTATORY_ENCODER_DT);
  int newClk = digitalRead(PIN_ROTATORY_ENCODER_CLK);
  if (newDt != newClk) {
    rotatoryEncoderPos++;
  } else {
    rotatoryEncoderPos--;
  }
  Serial.print("RotatoryEncoder: "); Serial.print((newDt != newClk) ? "> " : "< "); Serial.println(rotatoryEncoderPos);
}

void setup_rotatory_encoder() {
  pinMode(PIN_ROTATORY_ENCODER_SW, INPUT_PULLUP);
  pinMode(PIN_ROTATORY_ENCODER_DT, INPUT_PULLUP);
  pinMode(PIN_ROTATORY_ENCODER_CLK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTATORY_ENCODER_CLK), isr, CHANGE);

  lastRotatoryEncoderValue = digitalRead(PIN_ROTATORY_ENCODER_DT);
}

int getRotatoryEncoderButton() {
  return digitalRead(PIN_ROTATORY_ENCODER_SW) == LOW;
}

int getRotatoryEncoderPosition() {
  return rotatoryEncoderPos;
}

void resetRotatoryEncoderPosition() {
  rotatoryEncoderPos = 0;
}

void loop_rotatory_encoder() {
  
}
