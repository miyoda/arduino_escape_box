#include "src/outputs/serial.h"
#include "src/inputs/buttons.h"
#include "src/outputs/led_builtin.h"
#include "src/outputs/led_rgb.h"
#include "src/outputs/led_array.h"
#include "src/outputs/lcd.h"
#include "src/outputs/buzzer.h"
#include "src/game.h"

void setup() {
  setup_serial();
  setup_buttons();
  setup_led_builtin();
  setup_led_rgb();
  setup_led_array();
  setup_lcd();
  setup_buzzer();
  setup_game();
}

void loop() {
  loop_serial();
  loop_buttons();
  loop_lcd();
  loop_led_builtin();
  loop_led_rgb();
  loop_led_array();
  loop_buzzer();
  loop_game();
}
