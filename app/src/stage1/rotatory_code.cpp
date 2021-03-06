#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rotatory_code.h"
#include "../game.h"
#include "../inputs/rotatory_encoder.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../outputs/lcd.h"

static int VALID_ROTATORY_CODE[] = {7, 9, 3, 11};

static struct pt pt_rotatory_code, pt_rotatory_code_finish_game;
static bool rotatory_code_passed = false;
static unsigned long timeToFinishRotatoryCodeGame = 0;
static bool currentRotatoryRotationAsc;
static int currentRotatoryCodeDigit;
static int currentRotatoryCode[5];
char rotatoryCodeLcdText[20];

void setup_rotatory_code() {
  PT_INIT(&pt_rotatory_code);
  PT_INIT(&pt_rotatory_code_finish_game);
}

bool is_rotatory_code_passed() {
  return rotatory_code_passed;
}

void failRotatoryCodeGame() {
  timeToFinishRotatoryCodeGame = 0;
  resetDefaultLcdText();
  playMelody(MELODY_FAILURE);
}

void printLcd() {
  if (currentRotatoryCodeDigit == 0) {
    sprintf(rotatoryCodeLcdText, "ROT: %d", abs(currentRotatoryCode[0]));
  } else if (currentRotatoryCodeDigit == 1) {
    sprintf(rotatoryCodeLcdText, "ROT: %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]));
  } else if (currentRotatoryCodeDigit == 2) {
    sprintf(rotatoryCodeLcdText, "ROT: %d %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]), abs(currentRotatoryCode[2]));
  } else if (currentRotatoryCodeDigit >= 3) {
    sprintf(rotatoryCodeLcdText, "ROT: %d %d %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]), abs(currentRotatoryCode[2]), abs(currentRotatoryCode[3]));
  }
  setLcdLine0Text(rotatoryCodeLcdText);
}

int schedule_rotatory_code(struct pt *pt) {
  static int i;
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getRotatoryEncoderButton());
    currentRotatoryRotationAsc = false;
    currentRotatoryCodeDigit = 0;
    currentRotatoryCode[0] = 0;
    resetRotatoryEncoderPosition();
    printLcd();
    playNote(NOTE_G5);
    PT_SLEEP(pt, 100);
    stopBuzzer();
    PT_WAIT_UNTIL(pt, !getRotatoryEncoderButton());
    timeToFinishRotatoryCodeGame = millis() + 4000;  
    for(currentRotatoryCodeDigit=0; currentRotatoryCodeDigit<4;) {
      PT_SLEEP(pt, 100);
      int newPosition = getRotatoryEncoderPosition();
      if (newPosition != currentRotatoryCode[currentRotatoryCodeDigit]) {
        int newRotationAsc = newPosition < currentRotatoryCode[currentRotatoryCodeDigit] ? true : false;
        if (newRotationAsc != currentRotatoryRotationAsc) {
          resetRotatoryEncoderPosition();
          newPosition = 0;
          currentRotatoryCodeDigit++;
          currentRotatoryRotationAsc = newRotationAsc;
        }
        currentRotatoryCode[currentRotatoryCodeDigit] = newPosition;
        playNote(NOTE_G5);
        PT_SLEEP(pt, 20);
        stopBuzzer();
        printLcd();
        timeToFinishRotatoryCodeGame = millis() + 4000;
      }
      if (currentRotatoryCodeDigit >= 3) { // Check solution
        bool codeOk = true;
        for(i=0; i<4 && codeOk; i++) {
          codeOk = abs(currentRotatoryCode[i]) == VALID_ROTATORY_CODE[i];
        }
        if (codeOk) {
          playMelody(MELODY_SUCCESS);
          PT_SLEEP(pt, 500);
          resetDefaultLcdText();
          rotatory_code_passed = true;
          PT_RESTART(pt);
        } else if (currentRotatoryCodeDigit == 4){
          failRotatoryCodeGame();
        }
      }
    }
  }
  PT_END(pt);
}

int schedule_rotatory_code_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishRotatoryCodeGame != 0 && (getRotatoryEncoderButton() || (millis() > timeToFinishRotatoryCodeGame)));
    failRotatoryCodeGame();
    PT_RESTART(&pt_rotatory_code);
  }
  PT_END(pt);
}

void loop_rotatory_code() {
  if (!rotatory_code_passed) {
    PT_SCHEDULE(schedule_rotatory_code(&pt_rotatory_code));
    PT_SCHEDULE(schedule_rotatory_code_finish_game(&pt_rotatory_code_finish_game));
  }
}
