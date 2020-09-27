#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_rgb.h"
#include "../outputs/led_array.h"
#include "stage2.h"
#include "simon_dice.h"
#include "../stageX/keyboard_pin.h"


static struct pt pt_stage2_status; 

void setup_stage2() {
  PT_INIT(&pt_stage2_status);

  setup_simon_dice();
}

int schedule_stage2_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(2000);
  led_array_set(0, LOW); // TODO ir_code
  led_array_set(1, LOW); // TODO tarjeta_rfid
  led_array_set(2, LOW); // TODO cable_union
  led_array_set(3, LOW); // simon_dice
  
  for(;;) {
    PT_SLEEP(pt, 1000);
    // TODO 3 leds
    if (is_simon_dice_passed()) {
      led_array_set(3, HIGH);
    }
    if (is_simon_dice_passed()) { // TODO 3 leds
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE3);
    }
  }
  
  PT_END(pt);
}

void loop_stage2() {
  loop_simon_dice();

  PT_SCHEDULE(schedule_stage2_status(&pt_stage2_status));
}