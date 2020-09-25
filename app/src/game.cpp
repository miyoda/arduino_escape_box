#include "protothreads.h"
#include "game.h"
#include "Arduino.h"
#include "outputs/lcd.h"
#include "outputs/buzzer.h"
#include "outputs/led_rgb.h"
#include "outputs/led_array.h"
#include "games/morse_button.h"

static struct pt pt_game_time, pt_game_status; 

enum GameStep{STEP1, STEP2};

static GameStep currentStep = STEP1;

void setup_game() {
  PT_INIT(&pt_game_time);
  PT_INIT(&pt_game_status);

  setup_morse_button();

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
    
    // TODO temporal change STEP2 automatically
    if (millis() > 4000) {
      currentStep = STEP2;
    }
    PT_SLEEP(pt, 30);
  }
  PT_END(pt);
}

int schedule_game_status(struct pt *pt) {
  static char text[20];
  PT_BEGIN(pt);
  for(;;) {
    setLcdLine0Text("Empieza la aventura!");
    playMelody(MELODY_FAILURE);
    led_rgb_red();
    led_array_showAnimation(2000);
    led_array_set(0, LOW);
    led_array_set(1, HIGH);
    led_array_set(2, LOW);
    led_array_set(3, HIGH);
    PT_WAIT_WHILE(pt, currentStep == STEP1);
    setLcdLine0Text("STEP 2");
    playMelody(MELODY_SUCCESS);
    led_rgb_blue();
    led_array_set(0, HIGH);
    led_array_set(1, LOW);
    led_array_set(2, HIGH);
    led_array_set(3, LOW);
    PT_WAIT_WHILE(pt, currentStep == STEP2);
  }
  PT_END(pt);
}

void loop_game() {
  PT_SCHEDULE(schedule_game_time(&pt_game_time));
  PT_SCHEDULE(schedule_game_status(&pt_game_status));
  if (currentStep == STEP2) {
    loop_morse_button();
  }
}