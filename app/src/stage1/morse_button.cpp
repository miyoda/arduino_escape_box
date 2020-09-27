#include <protothreads.h>
#include <Arduino.h>
#include "morse_button.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/buttons.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../utils/str_utils.h"
#include "../utils/morse.h"

static struct pt pt_morse_button, pt_morse_button_finish_alphabet, pt_morse_button_finish_game;
static bool morse_button_passed = false;
static char currentWord[7] = "";
static char currentMorseCode[6] = "";
static int t1, t2;
static unsigned long timeToFinishAlphabet = 0, timeToFinishGame = 0;

void setup_morse_button() {
  PT_INIT(&pt_morse_button);
  PT_INIT(&pt_morse_button_finish_alphabet);
  PT_INIT(&pt_morse_button_finish_game);
}

bool is_morse_button_passed() {
  return morse_button_passed;
}

char readio(int signal_len) {
  if (signal_len < 200) {
    return '.';
  } else {
    return '-';
  }
}

int schedule_morse_button(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, getButton1());
    playNote(NOTE_E5);
    t1 = millis();
    PT_WAIT_UNTIL(pt, !getButton1());
    stopBuzzer();
    t2 = millis();
    Serial.print(readio(t2-t1));
    append(currentMorseCode, readio(t2-t1));
    timeToFinishAlphabet = millis() + 600;
    timeToFinishGame = millis() + 5000;
  }
  PT_END(pt);
}

int schedule_morse_button_finish_alphabet(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishAlphabet != 0 && millis() > timeToFinishAlphabet);
    timeToFinishAlphabet = 0;
    strcat(currentWord, morse_to_char(currentMorseCode));
    strcpy(currentMorseCode, "");
    setLcdLine0Text(currentWord);
    PT_WAIT_UNTIL(pt, getButton1());
  }
  PT_END(pt);
}

int schedule_morse_button_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    PT_WAIT_UNTIL(pt, timeToFinishGame != 0 && (strlen(currentWord) >= 3 || millis() > timeToFinishGame));
    PT_SLEEP(pt, 1000);
    resetDefaultLcdText();
    if (strcmp(currentWord, "SOS") == 0) {
      playMelody(MELODY_SUCCESS);
      morse_button_passed = true;
    } else {
      playMelody(MELODY_FAILURE);
    }
    timeToFinishAlphabet = 0;
    timeToFinishGame = 0;
    strcpy(currentWord, "");
    strcpy(currentMorseCode, "");
  }
  PT_END(pt);
}

void loop_morse_button() {
  if (!morse_button_passed) {
    PT_SCHEDULE(schedule_morse_button(&pt_morse_button));
    PT_SCHEDULE(schedule_morse_button_finish_alphabet(&pt_morse_button_finish_alphabet));
    PT_SCHEDULE(schedule_morse_button_finish_game(&pt_morse_button_finish_game));
  }
}
