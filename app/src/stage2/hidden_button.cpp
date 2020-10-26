#include <protothreads.h>
#include <time.h>
#include <stdlib.h>
#include <Arduino.h>
#include "hidden_button.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/buttons.h"
#include "../outputs/buzzer.h"


static struct pt pt_hidden_button;
static bool hidden_button_passed = false;

void setup_hidden_button() {
  PT_INIT(&pt_hidden_button);
}

bool is_hidden_button_passed() {
  return hidden_button_passed;
}

int schedule_hidden_button(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getButtonHidden());
    PT_SLEEP(pt, 100);
    if (getButtonHidden()) {
      PT_SLEEP(pt, 20);
      if (getButtonHidden()) {
        playMelody(MELODY_SUCCESS);
        hidden_button_passed = true;
      }
    }
  }
  PT_END(pt);
}

void loop_hidden_button() {
  if (!hidden_button_passed) {
    PT_SCHEDULE(schedule_hidden_button(&pt_hidden_button));
  }
}
