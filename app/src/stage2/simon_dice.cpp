#include <protothreads.h>
#include <time.h>
#include <stdlib.h>
#include <Arduino.h>
#include "simon_dice.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/buttons.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_simon.h"


static int const SIMON_DICE_LENGTH = 15;
static struct pt pt_simon_dice,pt_simon_dice_finish_game;
static bool simon_dice_passed = false;
static int currentSimonDiceCode[SIMON_DICE_LENGTH];
static unsigned long timeToFinishSimonDiceGame = 0;

void setRandomSimonDiceCode() {
  for (int i=0; i<SIMON_DICE_LENGTH; i++) {
    currentSimonDiceCode[i] = rand()%4;
    Serial.println(currentSimonDiceCode[i]);
  }
}

void setup_simon_dice() {
  srand(time(NULL));  
  PT_INIT(&pt_simon_dice);
  PT_INIT(&pt_simon_dice_finish_game);
}

bool is_simon_dice_passed() {
  return simon_dice_passed;
}

int getNoteOfLed(int led) {
  switch (led)
  {
  case 0:
    return NOTE_A5;
  case 1:
    return NOTE_C5;
  case 2:
    return NOTE_E5;
  case 3:
    return NOTE_G5;
  default:
    break;
  }
}

int getJoystickLed(int joystickX, int joystickY) {
  if ((joystickX > 490 && joystickX < 534) ||
    (joystickY > 490 && joystickY < 534)
  ) {
    return -1;
  }
  if (joystickX < 512) {
    if (joystickY < 512) {
      return 0;
    } else {
      return 1;
    }
  } else {
    if (joystickY < 512) {
      return 2;
    } else {
      return 3;
    }
  }
  return 0;
}


void failGame() {
  timeToFinishSimonDiceGame = 0;
  resetDefaultLcdText();
  playMelody(MELODY_FAILURE);
}

int schedule_simon_dice(struct pt *pt) {
  static char i, j;
  static int currentSimonLed;
  static bool fail;
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getJoystickSw());
    setRandomSimonDiceCode();
    fail = false;
    for (i=0; i<SIMON_DICE_LENGTH && !fail; i++) {
      for (j=0; j<=i; j++) {
        led_simon_set(currentSimonDiceCode[j], HIGH);
        playNote(getNoteOfLed(currentSimonDiceCode[j]));
        PT_SLEEP(pt, 500);
        led_simon_set(currentSimonDiceCode[j], LOW);
        stopBuzzer();
        PT_SLEEP(pt, 200);
      }
      for (j=0; j<=i && !fail; j++) {
        timeToFinishSimonDiceGame = millis() + 3000;
        PT_WAIT_UNTIL(pt, (currentSimonLed = getJoystickLed(getJoystickX(), getJoystickY())) != -1);
        timeToFinishSimonDiceGame = 0;
        led_simon_set(currentSimonLed, HIGH);
        playNote(getNoteOfLed(currentSimonLed));
        PT_SLEEP(pt, 500);
        led_simon_set(currentSimonLed, LOW);
        stopBuzzer();
        PT_SLEEP(pt, 200);
        if (currentSimonDiceCode[j] != currentSimonLed) {
          fail = true;
          failGame();
        }
      }
      PT_SLEEP(pt, 600);
    }
    if (!fail) {
      playMelody(MELODY_SUCCESS);
      simon_dice_passed = true;
    }

  }
  PT_END(pt);
}

int schedule_simon_dice_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishSimonDiceGame != 0 && millis() > timeToFinishSimonDiceGame);
    failGame();
    PT_RESTART(&pt_simon_dice);
  }
  PT_END(pt);
}

void loop_simon_dice() {
  if (!simon_dice_passed) {
    PT_SCHEDULE(schedule_simon_dice(&pt_simon_dice));
    PT_SCHEDULE(schedule_simon_dice_finish_game(&pt_simon_dice_finish_game));
  }
}
