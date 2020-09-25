#include "protothreads.h"
#include "../inputs/buttons.h"
#include "morse_button.h"
#include "Arduino.h"
#include "../outputs/buzzer.h"

static struct pt pt_morse_button; 

/*String morsecode[] ={
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
    "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
    "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
    "-.--", "--..", " "
};
String alpha [] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p",
"q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "};
*/
void setup_morse_button() {
  PT_INIT(&pt_morse_button);
}

int schedule_morse_button(struct pt *pt) {
  PT_BEGIN(pt);

  for(;;) {

    PT_SLEEP(pt, 40);
    Serial.println(getButton1());
  }

  /*
  
  PT_WAIT_WHILE(pt, getButton1() == HIGH);
  //playNote(NOTE_G6);

  PT_WAIT_WHILE(pt, getButton1() == LOW);
  playNote(NOTE_A6);
  t1 = millis();
  
  PT_WAIT_WHILE(pt, getButton1() == LOW);
  t2 = millis();
  digitalWrite(ledPin, LOW);                //LED off on button release
  signal_len = t2 - t1;                     //time for which button is pressed
  if (signal_len > 50)                      //to account for switch debouncing
  {
    code += readio();                       //function to read dot or dash
  }*/
  PT_END(pt);
}

void loop_morse_button() {
  //PT_SCHEDULE(schedule_morse_button(&pt_morse_button));
}
