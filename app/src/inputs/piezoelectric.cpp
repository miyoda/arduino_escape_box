#include <protothreads.h>
#include <Arduino.h>

int const PINA_PIEZOELECTRIC_AO = 1;

void setup_piezoelectric() {

}

int getPiezoelectricAnalog() {
  return analogRead(PINA_PIEZOELECTRIC_AO);
}

void loop_piezoelectric() {
  
}