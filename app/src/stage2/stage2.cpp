#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "stage2.h"
#include "simon_dice.h"
#include "rfid_identify_card.h"
#include "wire_bonding.h"
#include "hidden_button.h"
#include "../inputs/buttons.h"
#include "../stageX/keyboard_code.h"


static struct pt pt_stage2_status; 

void setup_stage2() {
  PT_INIT(&pt_stage2_status);

  setup_simon_dice();
  setup_rfid_identify_card();
  setup_wire_bonding();
  setup_hidden_button();
}

int schedule_stage2_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(2000);
  led_array_set(0, LOW);
  led_array_set(1, LOW);
  led_array_set(2, LOW);
  led_array_set(3, LOW);
  
  for(;;) {
    PT_SLEEP(pt, 200);
    bool allOk = true;

    if (is_simon_dice_passed()) {
      led_array_set(0, HIGH);
    } else {
      allOk = false;
    }

    if (is_wire_bonding_passed()) {
      led_array_set(1, HIGH);
    } else {
      allOk = false;
    }

    if (is_rfid_identify_card_passed()) {
      led_array_set(2, HIGH);
    } else {
      allOk = false;
    }

    if (is_hidden_button_passed()) {
      led_array_set(3, HIGH);
    } else {
      allOk = false;
    }

    if (allOk) {
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE3);
    }
  }
  
  PT_END(pt);
}

void loop_stage2() {
  loop_simon_dice();
  loop_rfid_identify_card();
  loop_wire_bonding();
  loop_hidden_button();

  PT_SCHEDULE(schedule_stage2_status(&pt_stage2_status));
}