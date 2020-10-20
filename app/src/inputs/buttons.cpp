#include <protothreads.h>
#include <Arduino.h>

int const PIN_BUTTON_EXTERNAL = 47;
int const PIN_BUTTON_HIDDEN = 7;
int const PIN_WIRE1 = 9;
int const PIN_WIRE2 = 10;
int const PIN_JOYSTICK_SW = 23;
int const PINA_JOYSTICK_X = 14;
int const PINA_JOYSTICK_Y = 15;

int getButtonExternal() {
  return digitalRead(PIN_BUTTON_EXTERNAL) == LOW;
}

int getButtonHidden() {
  return digitalRead(PIN_BUTTON_HIDDEN) == LOW;
}

int getWire1() {
  return digitalRead(PIN_WIRE1) == LOW;
}

int getWire2() {
  return digitalRead(PIN_WIRE2) == LOW;
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
  pinMode(PIN_BUTTON_EXTERNAL, INPUT_PULLUP);
  pinMode(PIN_BUTTON_HIDDEN, INPUT_PULLUP);
  pinMode(PIN_WIRE1, INPUT_PULLUP);
  pinMode(PIN_WIRE2, INPUT_PULLUP);
  pinMode(PIN_JOYSTICK_SW , INPUT_PULLUP);
}

void loop_buttons() {

}
