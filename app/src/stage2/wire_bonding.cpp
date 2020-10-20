#include <protothreads.h>
#include <time.h>
#include <stdlib.h>
#include <Arduino.h>
#include "wire_bonding.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/buttons.h"
#include "../outputs/buzzer.h"


static struct pt pt_wire_bonding;
static bool wire_bonding_passed = false;

void setup_wire_bonding() {
  PT_INIT(&pt_wire_bonding);
}

bool is_wire_bonding_passed() {
  return wire_bonding_passed;
}

int schedule_wire_bonding(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getWire1() && getWire2());
    PT_SLEEP(pt, 100);
    if (getWire1() && getWire2()) {
      playMelody(MELODY_SUCCESS);
      wire_bonding_passed = true;
    }
  }
  PT_END(pt);
}

void loop_wire_bonding() {
  if (!wire_bonding_passed) {
    PT_SCHEDULE(schedule_wire_bonding(&pt_wire_bonding));
  }
}
