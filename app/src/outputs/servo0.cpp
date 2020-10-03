#include <protothreads.h>
#include "servo0.h"
#include <Arduino.h>
#include <Servo.h>

int const PIN_SERVO0 = 11;

Servo servo0;

void setServo0Position(int position) { // Position value should be between 0 and 180
  servo0.write(position);
}

void setup_servo0() {
  servo0.attach(PIN_SERVO0);
}

void loop_servo0() {
  
}

