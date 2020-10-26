#include <protothreads.h>
#include <Arduino.h>
#include "knock_code.h"
#include "../game.h"
#include "../inputs/piezoelectric.h"
#include "../outputs/buzzer.h"
#include "../outputs/lcd.h"


int const KNOCK_CODE_THRESHOLD = 12;
int const KNOCK_MIN_WAIT = 100;
int const KNOCK_MAX_DURATION = 6000;
static int VALID_KNOCK_CODE[] = {1, 0, 2, 0};

static struct pt pt_knock_code, pt_knock_code_finish_game;
static bool knock_code_passed = false;
static unsigned long timeToFinishKnockCodeGame = 0;
static int currentKnockCode[5];
char knockCodeLcdText[20];
static unsigned long knockPreviousTime = 0;

void setup_knock_code() {
  PT_INIT(&pt_knock_code);
  PT_INIT(&pt_knock_code_finish_game);
}

bool is_knock_code_passed() {
  return knock_code_passed;
}

void failKnockCodeGame(bool sound) {
  timeToFinishKnockCodeGame = 0;
  resetDefaultLcdText();
  if (sound) {
    playMelody(MELODY_FAILURE);
  }
}

void printLcd(int currentKnockCodeDigit) {
  if (currentKnockCodeDigit == -1) {
    sprintf(knockCodeLcdText, "TOC TOC: ");
  } else if (currentKnockCodeDigit == 0) {
    sprintf(knockCodeLcdText, "TOC TOC: %d", abs(currentKnockCode[0]));
  } else if (currentKnockCodeDigit == 1) {
    sprintf(knockCodeLcdText, "TOC TOC: %d %d", abs(currentKnockCode[0]), abs(currentKnockCode[1]));
  } else if (currentKnockCodeDigit == 2) {
    sprintf(knockCodeLcdText, "TOC TOC: %d %d %d", abs(currentKnockCode[0]), abs(currentKnockCode[1]), abs(currentKnockCode[2]));
  } else if (currentKnockCodeDigit >= 3) {
    sprintf(knockCodeLcdText, "TOC TOC: %d %d %d %d", abs(currentKnockCode[0]), abs(currentKnockCode[1]), abs(currentKnockCode[2]), abs(currentKnockCode[3]));
  }
  setLcdLine0Text(knockCodeLcdText);
}

void saveKnockCode(int currentKnockCodeDigit) {
  if (currentKnockCodeDigit != -1) {
    currentKnockCode[currentKnockCodeDigit] = (millis() - knockPreviousTime) / 1000;
  }
  knockPreviousTime = millis();
  timeToFinishKnockCodeGame = millis() + KNOCK_MAX_DURATION;
  printLcd(currentKnockCodeDigit);
}

int schedule_knock_code(struct pt *pt) {
  static int i;
  PT_BEGIN(pt);
  PT_SLEEP(pt, 3000);
  for(;;) {
    PT_WAIT_UNTIL(pt, getPiezoelectricAnalog() > KNOCK_CODE_THRESHOLD);
    saveKnockCode(-1);
    PT_SLEEP(pt, KNOCK_MIN_WAIT);

    PT_WAIT_UNTIL(pt, getPiezoelectricAnalog() > KNOCK_CODE_THRESHOLD);
    saveKnockCode(0);
    PT_SLEEP(pt, KNOCK_MIN_WAIT);

    PT_WAIT_UNTIL(pt, getPiezoelectricAnalog() > KNOCK_CODE_THRESHOLD);
    saveKnockCode(1);
    PT_SLEEP(pt, KNOCK_MIN_WAIT);

    PT_WAIT_UNTIL(pt, getPiezoelectricAnalog() > KNOCK_CODE_THRESHOLD);
    saveKnockCode(2);
    PT_SLEEP(pt, KNOCK_MIN_WAIT);

    PT_WAIT_UNTIL(pt, getPiezoelectricAnalog() > KNOCK_CODE_THRESHOLD);
    saveKnockCode(3);
    PT_SLEEP(pt, KNOCK_MIN_WAIT);
    
    PT_SLEEP(pt, 1000);
    bool codeOk = true;
    Serial.println("check");
    Serial.println(codeOk);
    for(int i=0; i<4 && codeOk; i++) {
      Serial.println(i);
      codeOk = currentKnockCode[i] == VALID_KNOCK_CODE[i];
      Serial.println(codeOk);
    }
    Serial.println("check end");
    Serial.println(codeOk);
    if (codeOk) {
      playMelody(MELODY_SUCCESS);
      PT_SLEEP(pt, 500);
      resetDefaultLcdText();
      knock_code_passed = true;
      PT_RESTART(pt);
    } else {
      failKnockCodeGame(true);
    }
  }
  PT_END(pt);
}

int schedule_knock_code_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishKnockCodeGame != 0 && (millis() > timeToFinishKnockCodeGame));
    failKnockCodeGame(false);
    PT_RESTART(&pt_knock_code);
  }
  PT_END(pt);
}

void loop_knock_code() {
  if (!knock_code_passed) {
    PT_SCHEDULE(schedule_knock_code(&pt_knock_code));
    PT_SCHEDULE(schedule_knock_code_finish_game(&pt_knock_code_finish_game));
  }
}
