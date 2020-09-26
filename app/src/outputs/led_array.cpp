#include <protothreads.h>
#include "led_array.h"
#include <Arduino.h>


static struct pt pt_led_array; 

int const PIN_LED_ARRAY[] = {34, 35, 36, 37};
int const NUM_LEDS = sizeof(PIN_LED_ARRAY) / sizeof(PIN_LED_ARRAY[0]);

int ledArrayStatus[] {LOW, LOW, LOW, LOW};
int currentLedArrayStatus[] = {LOW, LOW, LOW, LOW};
int showAnimationUntil = 0;

void led_array_set(int pos, int status) {
  Serial.print("led array["); Serial.print(pos); Serial.print("] = "); Serial.println(status);
  ledArrayStatus[pos] = status;
}

void led_array_showAnimation(int durationMillis) {
  showAnimationUntil = millis() + durationMillis;
  Serial.print("led array animation until "); Serial.println(showAnimationUntil);
}

void setup_led_array() {
  PT_INIT(&pt_led_array);

  for (int i=0; i<NUM_LEDS; i++) {
    pinMode(PIN_LED_ARRAY[i], OUTPUT);
  }
}

void digitalWriteLed(int pos, int newStatus) {
  Serial.print("digitalWriteLed "); Serial.print(pos); Serial.print("="); Serial.println(newStatus);
  currentLedArrayStatus[pos] = newStatus;
  if (newStatus == HIGH || newStatus == LOW) {
    digitalWrite(PIN_LED_ARRAY[pos], newStatus);
  } else {
    analogWrite(PIN_LED_ARRAY[pos], newStatus);
  }
}

void digitalWriteLedArray(int status0, int status1, int status2, int status3) {
  Serial.println("digitalWriteLedArray");
  digitalWriteLed(0, status0);
  digitalWriteLed(1, status1);
  digitalWriteLed(2, status2);
  digitalWriteLed(3, status3);
}

int schedule_led_array(struct pt *pt) {
  PT_BEGIN(pt);
  for(;;) {
    while (showAnimationUntil > 0) {
      digitalWriteLedArray(HIGH,LOW,LOW,LOW);
      PT_SLEEP(pt, 50);
      digitalWriteLedArray(LOW,HIGH,LOW,LOW);
      PT_SLEEP(pt, 50);
      digitalWriteLedArray(LOW,LOW,HIGH,LOW);
      PT_SLEEP(pt, 50);
      digitalWriteLedArray(LOW,LOW,LOW,HIGH);
      PT_SLEEP(pt, 50);
      digitalWriteLedArray(LOW,LOW,HIGH,LOW);
      PT_SLEEP(pt, 50);
      digitalWriteLedArray(LOW,HIGH,LOW,LOW);
      PT_SLEEP(pt, 50);
      if (showAnimationUntil < millis()) {
        showAnimationUntil = 0;
      }
    }
    for (int i=0; i<NUM_LEDS; i++) {
      if (ledArrayStatus[i] != currentLedArrayStatus[i]) {
        digitalWriteLed(i, ledArrayStatus[i]);
      }
    }
    PT_WAIT_WHILE(pt, (memcmp(ledArrayStatus, currentLedArrayStatus, sizeof(ledArrayStatus)) == 0) && (showAnimationUntil == 0));
  }
  PT_END(pt);
}

void loop_led_array() {
  PT_SCHEDULE(schedule_led_array(&pt_led_array));
}

