#include <Arduino.h>
#include "lib/keypad.h"
#include "keypad_abcd09.h"

const byte KEYPAD_ABCD_NUM_ROWS = 4;
const byte KEYPAD_ABCD_NUM_COLS = 4;

byte PIN_KEYPAD_ABCD_ROWS[KEYPAD_ABCD_NUM_ROWS] = {45, 44, 43, 42};
byte PIN_KEYPAD_ABCD_COLS[KEYPAD_ABCD_NUM_COLS] = {41, 40, 39, 38};


//define the two-dimensional array on the buttons of the keypads
char KEYPAD_ABCD_KEYS[KEYPAD_ABCD_NUM_ROWS][KEYPAD_ABCD_NUM_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad abcd09Keypad = Keypad(makeKeymap(KEYPAD_ABCD_KEYS), PIN_KEYPAD_ABCD_ROWS, PIN_KEYPAD_ABCD_COLS, KEYPAD_ABCD_NUM_ROWS, KEYPAD_ABCD_NUM_COLS); 

char get_keypad_abcd09_key() {
  return abcd09Keypad.getKey();
}

void setup_keypad_abcd09() {
  
}

void loop_keypad_abcd09() {

}

