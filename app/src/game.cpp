#include <protothreads.h>
#include "game.h"
#include <Arduino.h>
#include "outputs/lcd.h"
#include "outputs/buzzer.h"
#include "outputs/led_rgb.h"
#include "outputs/led_array.h"
#include "stageX/keyboard_pin.h"
#include "stage1/stage1.h"
#include "stage2/stage2.h"
#include "stage3/stage3.h"

static struct pt pt_game_time; 

static GameStage currentStage;

GameStage getCurrentStage() {
  return currentStage;
}

void resetDefaultLcdText() {
  if (currentStage == STAGE1) {
    setLcdLine0Text("Stage 1");
  } else if (currentStage == STAGE2) {
    setLcdLine0Text("Stage 2");
  } else if (currentStage == STAGE3) {
    setLcdLine0Text("Stage 3");
  } else if (currentStage == STAGE4) {
    setLcdLine0Text("Stage 4");
  }
}

void setCurrentStage(GameStage gameStage) {
  currentStage = gameStage;
  resetDefaultLcdText();
  if (currentStage == STAGE1) {
    setup_stage1();
  } else if (currentStage == STAGE2) {
    setup_stage2();
  } else if (currentStage == STAGE3) {
    setup_stage3();
  } else if (currentStage == STAGE4) {
    
  }
}

void setup_game() {
  PT_INIT(&pt_game_time);

  setup_keyboard_pin();

  pinMode(LED_BUILTIN, OUTPUT);

  playMelody(MELODY_INIT);
  setCurrentStage(STAGE2); // TODO DEBUG
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

  loop_keyboard_pin();
  if (currentStage == STAGE1) {
    loop_stage1();
  } else if (currentStage == STAGE2) {
    loop_stage2();
  } else if (currentStage == STAGE3) {
    loop_stage3();
  }
}