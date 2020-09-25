#include "protothreads.h"
#include "game.h"
#include "stage2.h"
#include "Arduino.h"
#include "outputs/lcd.h"
#include "outputs/buzzer.h"
#include "outputs/led_rgb.h"
#include "outputs/led_array.h"
#include "games/morse_button.h"


static struct pt pt_stage2_status; 

void setup_stage2() {
  PT_INIT(&pt_stage2_status);
}

int schedule_stage2_status(struct pt *pt) {
  PT_BEGIN(pt);
  setLcdLine0Text(STAGE2_LCD_TEXT);
  led_array_showAnimation(2000);
  led_array_set(0, LOW);
  led_array_set(1, LOW);
  led_array_set(2, LOW);
  led_array_set(3, LOW);
  PT_WAIT_UNTIL(pt, false); // TODO more
  Serial.println("Stage 1 FINISHED");
  setCurrentStage(STAGE3);
  
  PT_END(pt);
}

void loop_stage2() {
  PT_SCHEDULE(schedule_stage2_status(&pt_stage2_status));
}