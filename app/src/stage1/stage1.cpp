#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../outputs/servo0.h"
#include "stage1.h"
#include "morse_button.h"
#include "rfid_identify_keychain.h"
#include "rotatory_code.h"
#include "knock_code.h"

static struct pt pt_stage1_status; 

void setup_stage1() {
  PT_INIT(&pt_stage1_status);

  setup_morse_button();
  setup_rfid_identify_keychain();
  setup_rotatory_code();
  setup_knock_code();

  setServo0Position(0);
}

int schedule_stage1_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(5000);
  led_array_set(0, LOW);
  led_array_set(1, LOW);
  led_array_set(2, LOW);
  led_array_set(3, LOW);
  
  for(;;) {

    PT_SLEEP(pt, 1000);
    bool allOk = true;
    if (is_morse_button_passed()) {
      led_array_set(0, HIGH);
    } else {
      allOk = false;
    }
    if (is_rfid_identify_keychain_passed()) {
      led_array_set(1, HIGH);
    } else {
      allOk = false;
    }
    if (is_rotatory_code_passed()) {
      led_array_set(2, HIGH);
    } else {
      allOk = false;
    }
    if (is_knock_code_passed()) {
      led_array_set(3, HIGH);
    } else {
      allOk = false;
    }
    
    if (allOk) {
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE2);
    }
  }
  PT_END(pt);
}

void loop_stage1() {
  loop_morse_button();
  loop_rfid_identify_keychain();
  loop_rotatory_code();
  loop_knock_code();

  PT_SCHEDULE(schedule_stage1_status(&pt_stage1_status));
}