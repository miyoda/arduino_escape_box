#include "src/outputs/serial.h"
#include "src/outputs/led_builtin.h"
#include "src/outputs/led_rgb.h"
#include "src/outputs/led_array.h"
#include "src/outputs/led_simon.h"
#include "src/outputs/lcd.h"
#include "src/outputs/buzzer.h"
#include "src/inputs/buttons.h"
#include "src/inputs/rfid.h"
#include "src/inputs/mpu.h"
#include "src/inputs/keypad_abcd09.h"
#include "src/game.h"

void setup() {
  // setup outputs
  setup_serial();
  setup_led_builtin();
  setup_led_rgb();
  setup_led_array();
  setup_led_simon();
  setup_lcd();
  setup_buzzer();

  // setup inputs
  setup_buttons();
  setup_rfid();
  setup_mpu();
  setup_keypad_abcd09();

  setup_game();
}

void loop() {
  // loop outputs
  loop_serial();
  loop_lcd();
  loop_led_builtin();
  loop_led_rgb();
  loop_led_array();
  loop_led_simon();
  loop_buzzer();

  // loop inputs
  loop_rfid();
  loop_buttons();
  loop_mpu();
  loop_keypad_abcd09();

  loop_game();
}
