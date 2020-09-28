#include <protothreads.h>
#include <Arduino.h>
#include "keyboard_pin.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/keypad_abcd09.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../utils/str_utils.h"

static struct pt pt_keyboard_pin;
static bool keyboard_pin3A_passed = false;
static bool keyboard_pin3B_passed = false;
static bool keyboard_pin3C_passed = false;
static bool keyboard_pin3D_passed = false;
static char pressedKey0, pressedKey1, pressedKey2, pressedKey3, pressedKey4;

void setup_keyboard_pin() {
  PT_INIT(&pt_keyboard_pin);
}

bool is_keyboard_pin3A_passed() {
  return keyboard_pin3A_passed;
}
bool is_keyboard_pin3B_passed() {
  return keyboard_pin3B_passed;
}
bool is_keyboard_pin3C_passed() {
  return keyboard_pin3C_passed;
}
bool is_keyboard_pin3D_passed() {
  return keyboard_pin3D_passed;
}

bool isPin(char pin[6]) {
  return pressedKey0 == pin[0]
    && pressedKey1 == pin[1]
    && pressedKey2 == pin[2]
    && pressedKey3 == pin[3]
    && pressedKey4 == pin[4];
}

int schedule_keyboard_pin(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    Serial.println("schedule_keyboard_pin");
    PT_WAIT_UNTIL(pt, pressedKey0 = get_keypad_abcd09_key());
    if (pressedKey0 == 'A' || pressedKey0 == 'B' || pressedKey0 == 'C' || pressedKey0 == 'D' || pressedKey0 == '*' || pressedKey0 == '#') {
      playMelody(MELODY_KEY_PRESS);
      PT_WAIT_UNTIL(pt, pressedKey1 = get_keypad_abcd09_key());
      playMelody(MELODY_KEY_PRESS);
      PT_WAIT_UNTIL(pt, pressedKey2 = get_keypad_abcd09_key());
      playMelody(MELODY_KEY_PRESS);
      PT_WAIT_UNTIL(pt, pressedKey3 = get_keypad_abcd09_key());
      playMelody(MELODY_KEY_PRESS);
      PT_WAIT_UNTIL(pt, pressedKey4 = get_keypad_abcd09_key());
      playMelody(MELODY_KEY_PRESS);

      if (getCurrentStage() == STAGE3 && isPin("A0003")) {
        playMelody(MELODY_SUCCESS);
        keyboard_pin3A_passed = true;
      } else if (getCurrentStage() == STAGE3 && isPin("B0003")) {
        playMelody(MELODY_SUCCESS);
        keyboard_pin3B_passed = true;
      } else if (getCurrentStage() == STAGE3 && isPin("C0003")) {
        playMelody(MELODY_SUCCESS);
        keyboard_pin3C_passed = true;
      } else if (getCurrentStage() == STAGE3 && isPin("D0003")) {
        playMelody(MELODY_SUCCESS);
        keyboard_pin3D_passed = true;
      } else if (isPin("*0001")) {
        setCurrentStage(STAGE1);
      } else if (isPin("*0002")) {
        setCurrentStage(STAGE2);
      } else if (isPin("*0003")) {
        setCurrentStage(STAGE3);
      } else if (isPin("*0004")) {
        setCurrentStage(STAGE4);
      } else if (isPin("*0005")) {
        setCurrentStage(STAGE_WIN);
      } else {
        playMelody(MELODY_FAILURE);
      }
    }
  }
  PT_END(pt);
}

void loop_keyboard_pin() {
  PT_SCHEDULE(schedule_keyboard_pin(&pt_keyboard_pin));
}
