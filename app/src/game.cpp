#include <protothreads.h>
#include "game.h"
#include <Arduino.h>
#include "outputs/lcd.h"
#include "outputs/buzzer.h"
#include "outputs/led_rgb.h"
#include "outputs/led_array.h"
#include "stage1.h"
#include "stage2.h"

static struct pt pt_game_time, pt_game_status; 

static GameStage currentStage = STAGE1;

GameStage getCurrentStage() {
  return currentStage;
}

void setCurrentStage(GameStage gameStage) {
  currentStage = gameStage;
}

void setup_game() {
  PT_INIT(&pt_game_time);
  PT_INIT(&pt_game_status);

  setup_stage1();
  setup_stage2();

  pinMode(LED_BUILTIN, OUTPUT);
}

void getFormatedTime(char *text) {
  unsigned long milliseconds = millis();
  int seconds = milliseconds / 1000;
  milliseconds %= 1000;
  int minutes = seconds / 60;
  seconds %= 60;
  int hours = minutes / 60;
  minutes %= 60;
  sprintf(text, "%dh %dm %ds %d",  hours, minutes, seconds, milliseconds);
}

int schedule_game_time(struct pt *pt) {
  static char timeText[30];
  PT_BEGIN(pt);
  getFormatedTime(timeText);
  setLcdLine1Text(timeText);
  for(;;) {
    getFormatedTime(timeText);
    
    PT_SLEEP(pt, 30);
  }
  PT_END(pt);
}

void loop_game() {
  PT_SCHEDULE(schedule_game_time(&pt_game_time));
  if (currentStage == STAGE1) {
    loop_stage1();
  }
  if (currentStage == STAGE2) {
    loop_stage2();
  }
}