#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rotatory_code.h"
#include "../game.h"
#include "../inputs/rotatory_encoder.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"

static struct pt pt_rotatory_code, pt_rotatory_code_finish_game;
static bool rotatory_code_passed = false;
static unsigned long timeToFinishRotatoryCodeGame = 0;
static Rotation lastRotation = Rotation.NONE;
static int currentRotatoryPosition = 0;
static int currentRotatoryCode[5];
static int VALID_ROTATORY_CODE = {34, 12, 6, 3};

//static byte whiteCardUUID[4] = {0x69, 0x6C, 0x58, 0xB3}; // Lavero
static byte keychainCardUUID[4] = {0x43, 0x46, 0xE2, 0x39}; // Tarjeta

void setup_rotatory_code() {
  PT_INIT(&pt_rotatory_code);
  PT_INIT(&pt_rotatory_code_finish_game);
}

bool is_rotatory_code_passed() {
  return rotatory_code_passed;
}

void failGame() {
  timeToFinishRotatoryCodeGame = 0;
  resetDefaultLcdText();
  playMelody(MELODY_FAILURE);
}

void printLcd() {
  char rotatoryCodeLcdText[20];
  if (currentRotatoryPosition == 0) {
    sprintf(rotatoryCodeLcdText, "%d", currentRotatoryCode[0]);
  } else if (currentRotatoryPosition == 1) {
    sprintf(rotatoryCodeLcdText, "%d %d", currentRotatoryCode[0], currentRotatoryCode[1]);
  } else if (currentRotatoryPosition == 2) {
    sprintf(rotatoryCodeLcdText, "%d %d %d", currentRotatoryCode[0], currentRotatoryCode[1], currentRotatoryCode[2]);
  } else if (currentRotatoryPosition > 3) {
    sprintf(rotatoryCodeLcdText, "%d %d %d %d", currentRotatoryCode[0], currentRotatoryCode[1], currentRotatoryCode[2], currentRotatoryCode[3]);
  }
  setLcdLine0Text(rotatoryCodeLcdText);
}

int schedule_rotatory_code(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT(pt, getRotatoryEncoderButton());
    playNote(NOTE_G5);
    PT_SLEEP(pt, 100);
    stopBuzzer();

    for(int i=0; i<4;) {
      timeToFinishRotatoryCodeGame = millis() + 4000;
      PT_SLEEP(pt, 10);
      Rotation newRotation = getRotatoryEncoderDirection();
      if (newRotation != Rotation.NONE) {
        if (newRotation != lastRotation) {
          i++;
          currentRotatoryPosition = 1;
          lastRotation = newRotation;
        } else {
          currentRotatoryPosition++;
        }
        currentRotatoryCode[i] = currentRotatoryPosition;
        printLcd()
      }
    }
    bool codeOk = true;
    for(int i=0; i<4 && codeOk; i++) {
      codeOk = currentRotatoryCode[i] = VALID_ROTATORY_CODE[i];
    }
    if (codeOk) {
      playMelody(MELODY_SUCCESS);
      rotatory_code_passed = true;
    } else {
      failGame();
    }
  }
  PT_END(pt);
}

int schedule_rotatory_code_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishRotatoryCodeGame != 0 && (millis() > timeToFinishRotatoryCodeGame));
    failGame();
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
