#include <protothreads.h>
#include "led_simon.h"
#include <Arduino.h>


static struct pt pt_led_simon; 

int const PIN_LED_SIMON[] = {30, 31, 32, 33};
int const NUM_LEDS = sizeof(PIN_LED_SIMON) / sizeof(PIN_LED_SIMON[0]);


void led_simon_set(int pos, int status) {
  Serial.print("led simon["); Serial.print(pos); Serial.print("] = "); Serial.println(status);
  digitalWrite(PIN_LED_SIMON[pos], status);
}

void setup_led_simon() {
  PT_INIT(&pt_led_simon);

  for (int i=0; i<NUM_LEDS; i++) {
    pinMode(PIN_LED_SIMON[i], OUTPUT);
  }
}

void loop_led_simon() {
  
}

