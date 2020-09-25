#include "protothreads.h"
#include "led_rgb.h"
#include "Arduino.h"


static struct pt pt_led_rgb; 

int const PIN_LED_RGB_RED = 2;
int const PIN_LED_RGB_GREEN = 3;
int const PIN_LED_RGB_BLUE = 4;

int redValue = 0, greenValue = 0, blueValue = 0;


void led_rgb_set(int red, int green, int blue) {
  Serial.print("led rgb = "); Serial.print(red); Serial.print(","); Serial.print(green); Serial.print(","); Serial.println(blue);
  redValue = red;
  greenValue = green;
  blueValue = blue;
}

void led_rgb_red() {
  led_rgb_set(255,0,0);
}

void led_rgb_green() {
  led_rgb_set(0,255,0);
}

void led_rgb_blue() {
  led_rgb_set(0,0,255);
}

void setup_led_rgb() {
  PT_INIT(&pt_led_rgb);

  pinMode(PIN_LED_RGB_RED, OUTPUT);
  pinMode(PIN_LED_RGB_GREEN, OUTPUT);
  pinMode(PIN_LED_RGB_BLUE, OUTPUT);
}

int schedule_led_rgb(struct pt *pt) {
  static int currentRed, currentGreen, currentBlue;
  PT_BEGIN(pt);
  for(;;) {
    currentRed = redValue;
    currentGreen = greenValue;
    currentBlue = blueValue;
    analogWrite(PIN_LED_RGB_RED, redValue);
    analogWrite(PIN_LED_RGB_GREEN, greenValue);
    analogWrite(PIN_LED_RGB_BLUE, blueValue);
    PT_WAIT_UNTIL(pt, currentRed != redValue || currentGreen != greenValue || currentBlue != blueValue);
  }
  PT_END(pt)
}

void loop_led_rgb() {
  PT_SCHEDULE(schedule_led_rgb(&pt_led_rgb));
}

