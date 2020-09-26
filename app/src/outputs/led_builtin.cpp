#include <protothreads.h>
#include "led_builtin.h"
#include <Arduino.h>

static struct pt pt_led_builtin; 

void setup_led_builtin() {
  PT_INIT(&pt_led_builtin);

  pinMode(LED_BUILTIN, OUTPUT);
}

int schedule_led_builtin(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    digitalWrite(LED_BUILTIN, HIGH);
    PT_SLEEP(pt, 1000);
    digitalWrite(LED_BUILTIN, LOW);
    PT_SLEEP(pt, 1000);
  }
  PT_END(pt)
}

void loop_led_builtin() {
  PT_SCHEDULE(schedule_led_builtin(&pt_led_builtin));
}

