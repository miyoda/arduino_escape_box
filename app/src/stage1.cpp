#include <protothreads.h>
#include "game.h"
#include "stage1.h"
#include <Arduino.h>
#include "outputs/lcd.h"
#include "outputs/buzzer.h"
#include "outputs/led_rgb.h"
#include "outputs/led_array.h"
#include "games/morse_button.h"
#include "games/rfid_identify.h"


static struct pt pt_stage1_status; 

void setup_stage1() {
  PT_INIT(&pt_stage1_status);

  setup_morse_button();
  setup_rfid_identify();
}

int schedule_stage1_status(struct pt *pt) {
  PT_BEGIN(pt);
  setLcdLine0Text(STAGE1_LCD_TEXT);
  playMelody(MELODY_INIT);
  led_array_showAnimation(2000);
  led_array_set(0, LOW); // morse_button
  led_array_set(1, LOW);
  led_array_set(2, LOW);
  led_array_set(3, LOW);
  PT_WAIT_UNTIL(pt, is_morse_button_passed()); // TODO more
  Serial.println("Stage 1 FINISHED");
  setCurrentStage(STAGE2);
  
  PT_END(pt);
}

void loop_stage1() {
  loop_morse_button();
  loop_rfid_identify();

  PT_SCHEDULE(schedule_stage1_status(&pt_stage1_status));
}