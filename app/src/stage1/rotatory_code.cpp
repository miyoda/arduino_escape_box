#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rotatory_code.h"
#include "../game.h"
#include "../inputs/rotatory_encoder.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../outputs/lcd.h"

static int VALID_ROTATORY_CODE[] = {1, 2, 3, 4};

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
    sprintf(rotatoryCodeLcdText, "CODE: %d", abs(currentRotatoryCode[0]));
  } else if (currentRotatoryCodeDigit == 1) {
    sprintf(rotatoryCodeLcdText, "CODE: %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]));
  } else if (currentRotatoryCodeDigit == 2) {
    sprintf(rotatoryCodeLcdText, "CODE: %d %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]), abs(currentRotatoryCode[2]));
  } else if (currentRotatoryCodeDigit >= 3) {
    sprintf(rotatoryCodeLcdText, "CODE: %d %d %d %d", abs(currentRotatoryCode[0]), abs(currentRotatoryCode[1]), abs(currentRotatoryCode[2]), abs(currentRotatoryCode[3]));
  }
  setLcdLine0Text(rotatoryCodeLcdText);
}

int schedule_rotatory_code(struct pt *pt) {
  static int i;
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getRotatoryEncoderButton());
    currentRotatoryRotationAsc = true;
    currentRotatoryCodeDigit = 0;
    currentRotatoryCode[0] = 0;
    resetRotatoryEncoderPosition();
    printLcd();
    playNote(NOTE_G5);
    PT_SLEEP(pt, 100);
    stopBuzzer();

    for(currentRotatoryCodeDigit=0; currentRotatoryCodeDigit<4;) {
      timeToFinishRotatoryCodeGame = millis() + 4000;
      PT_SLEEP(pt, 100);
      int newPosition = getRotatoryEncoderPosition() / 3;
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
      }
      if (currentRotatoryCodeDigit >= 3) { // Check solution
        bool codeOk = true;
        for(int i=0; i<4 && codeOk; i++) {
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
    PT_WAIT_UNTIL(pt, timeToFinishRotatoryCodeGame != 0 && (millis() > timeToFinishRotatoryCodeGame));
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
