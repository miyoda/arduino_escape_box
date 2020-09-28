#include <protothreads.h>
#include <Arduino.h>

int const PIN_BUTTON1 = 47;

int const PIN_JOYSTICK_SW = 23;
int const PINA_JOYSTICK_X = 14;
int const PINA_JOYSTICK_Y = 15;

int getButton1() {
  return digitalRead(PIN_BUTTON1) == LOW;
}

int getJoystickSw() {
  return digitalRead(PIN_JOYSTICK_SW) == LOW;
}

int getJoystickX() {
  return analogRead(PINA_JOYSTICK_X);
}

int getJoystickY() {
  return analogRead(PINA_JOYSTICK_Y);
}

void setup_buttons() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_JOYSTICK_SW , INPUT_PULLUP);
}

void loop_buttons() {

}
