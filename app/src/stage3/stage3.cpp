#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_rgb.h"
#include "../outputs/led_array.h"
#include "stage3.h"
#include "../stageX/keyboard_pin.h"


static struct pt pt_stage3_status; 

void setup_stage3() {
  PT_INIT(&pt_stage3_status);
}

int schedule_stage3_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(2000);
  led_array_set(0, LOW);
  led_array_set(1, LOW);
  led_array_set(2, LOW);
  led_array_set(3, LOW);
  
  for(;;) {
    PT_SLEEP(pt, 1000);
    if (is_keyboard_pin3A_passed()) {
      led_array_set(0, HIGH);
    }
    if (is_keyboard_pin3B_passed()) {
      led_array_set(1, HIGH);
    }
    if (is_keyboard_pin3C_passed()) {
      led_array_set(2, HIGH);
    }
    if (is_keyboard_pin3D_passed()) {
      led_array_set(3, HIGH);
    }
    if (is_keyboard_pin3A_passed() && is_keyboard_pin3B_passed() && is_keyboard_pin3C_passed() && is_keyboard_pin3D_passed()) {
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE3);
    }
  }
  
  PT_END(pt);
}

void loop_stage3() {
  PT_SCHEDULE(schedule_stage3_status(&pt_stage3_status));
}