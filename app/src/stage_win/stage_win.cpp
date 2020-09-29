#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "stage_win.h"
#include "../stageX/keyboard_code.h"


static struct pt pt_stage_win_status; 

void setup_stage_win() {
  PT_INIT(&pt_stage_win_status);
}

int schedule_stage_win_status(struct pt *pt) {
  PT_BEGIN(pt);
  
  for(;;) {
    PT_SLEEP(pt, 1000);
    led_array_set(0, LOW);
    led_array_set(1, LOW);
    led_array_set(2, LOW);
    led_array_set(3, LOW);
    PT_SLEEP(pt, 1000);
    led_array_set(0, HIGH);
    led_array_set(1, HIGH);
    led_array_set(2, HIGH);
    led_array_set(3, HIGH);
  }
  
  PT_END(pt);
}

void loop_stage_win() {
  PT_SCHEDULE(schedule_stage_win_status(&pt_stage_win_status));
}