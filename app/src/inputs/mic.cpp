#include <protothreads.h>
#include <Arduino.h>

/*+
  Sound sensor module.
  The sensitivity is adjusted by potentiometer.
  There is a tiny screw on the blue potentiometer block that you can use for adjustment. Turning that
  clockwise lowers the potentiometer value, while counter-clockwise raises the potentiometer value.
  Use the potentiometer to adjust the Sound Sensor sensitivity. Turn the potentiometer
  several rotations until you see the LED2 extinguish (or just faintly blink). This might be slightly greater than
  500, if you are also watching Serial Monitor (inital adjustment), or, Serial Plotter (the latter is prefererd for observation).
  Special thanks to user CRomer, for his input and hard work!
*/

int const PIN_MIC_DO = 8;
int const PINA_MIC_AO = 0;

void setup_mic() {
  pinMode(PIN_MIC_DO, INPUT);
}

int getMicAnalog() {
  return analogRead(PINA_MIC_AO);
}

int getMicDigital() {
  return digitalRead(PIN_MIC_DO);
}

void loop_mic() {
  
}
