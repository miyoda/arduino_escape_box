#include <protothreads.h>
#include "../game.h"
#include <Arduino.h>
#include "../inputs/buttons.h"
#include "../inputs/rfid.h"
#include "../inputs/mic.h"
#include "../outputs/lcd.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_rgb.h"
#include "../outputs/led_array.h"
#include "stage4.h"
#include "../stageX/keyboard_pin.h"


static struct pt pt_stage4_status; 

void setup_stage4() {
  PT_INIT(&pt_stage4_status);
}

int schedule_stage4_status(struct pt *pt) {
  PT_BEGIN(pt);
  led_array_showAnimation(2000);

  for(;;) {
    PT_SLEEP(pt, 200);
    bool allOk = true;

    if (getButton1()) {
      led_array_set(0, HIGH);
    } else {
      allOk = false;
      led_array_set(0, LOW);
    }

    Serial.println(getMicDigital());
    Serial.println(getMicAnalog());
    if (getMicDigital()) {
      led_array_set(1, HIGH);
    } else {
      allOk = false;
      led_array_set(1, LOW);
    }
    if (false) { // TODO led orientation N?
      led_array_set(2, HIGH);
    } else {
      allOk = false;
      led_array_set(2, LOW);
    }
    if (getJoystickSw() == LOW) {
      led_array_set(3, HIGH);
    } else {
      allOk = false;
      led_array_set(3, LOW);
    }

    if (allOk) {
      PT_SLEEP(pt, 1000);
      setCurrentStage(STAGE_WIN);
    }
  }
  
  PT_END(pt);
}

void loop_stage4() {
  PT_SCHEDULE(schedule_stage4_status(&pt_stage4_status));
}