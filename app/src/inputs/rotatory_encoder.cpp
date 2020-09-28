#include <protothreads.h>
#include <Arduino.h>
#include "rotatory_encoder.h"

/*+
  Rotatory encoder module.
  https://zaragozamakerspace.com/index.php/introduccion-al-rotary-encoder/
*/

enum Rotation {NONE, CLOCKWISE, COUNTERCLOCKWISE};

int const PIN_ROTATORY_ENCODER_SW = 11;
int const PIN_ROTATORY_ENCODER_DT = 12;
int const PIN_ROTATORY_ENCODER_CLK = 13;

static int lastRotatoryEncoderValue = 0;

void setup_rotatory_encoder() {
  pinMode (PIN_ROTATORY_ENCODER_SW, INPUT_PULLUP);
  pinMode (PIN_ROTATORY_ENCODER_CLK, INPUT);
  pinMode (PIN_ROTATORY_ENCODER_DT, INPUT);

  lastRotatoryEncoderValue = digitalRead(PIN_ROTATORY_ENCODER_DT);
}

int getRotatoryEncoderButton() {
  return digitalRead(PIN_ROTATORY_ENCODER_SW) == LOW;
}

Rotation getRotatoryEncoderDirection() {
  int dt = digitalRead(PIN_ROTATORY_ENCODER_DT);
  if (lastRotatoryEncoderValue != dt) {
    lastRotatoryEncoderValue = dt;
    if (digitalRead(PIN_ROTATORY_ENCODER_CLK) != dt) {
      return Rotation.CLOCKWISE;
    } else {
      return Rotation.COUNTERCLOCKWISE;
    }
  } else {
    return Rotation.NONE;
  }
}

void loop_rotatory_encoder() {
  
}
