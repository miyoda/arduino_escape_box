#include "protothreads.h"
#include "buzzer.h"
#include "Arduino.h"

// TODO use RTTTL melodies https://github.com/cefn/non-blocking-rtttl-arduino

int const PIN_ACTIVE_BUZZER = 6;

static Melody melody;

static struct pt pt_buzzer; 

void playMelody(Melody newMelody) {
  Serial.println("play melody");
  melody = newMelody;
}

void playNote(int note) {
  playMelody({
    .notes = {note},
    .durations = {999999999},
    .length = 1
  });
}

void stopBuzzer() {
  Serial.println("play melody");
  playMelody(MELODY_SILENT);
  noTone(PIN_ACTIVE_BUZZER);
}

void setup_buzzer() {
  PT_INIT(&pt_buzzer);

  stopBuzzer();
  pinMode(PIN_ACTIVE_BUZZER, OUTPUT);
}

int schedule_buzzer(struct pt *pt) {
  static int melodyPos;
  static int duration;
  PT_BEGIN(pt);
  for(;;) {
    for (melodyPos = 0; melodyPos < melody.length; melodyPos++) {
      duration = melody.durations[melodyPos];
      tone(PIN_ACTIVE_BUZZER, melody.notes[melodyPos], duration);
      PT_SLEEP(pt, duration * 1.3);
    }
    melody = MELODY_SILENT;
    PT_WAIT_UNTIL(pt, melody.length);
  }
  PT_END(pt)
}

void loop_buzzer() {
  PT_SCHEDULE(schedule_buzzer(&pt_buzzer));
}

