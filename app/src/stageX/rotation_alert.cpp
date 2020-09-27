#include <protothreads.h>
#include <Arduino.h>
#include "rotation_alert.h"
#include "../game.h"
#include "../outputs/lcd.h"
#include "../inputs/mpu.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"
#include "../utils/str_utils.h"

static struct pt pt_rotation_alert;

static bool rotation_alert_active = false;

void setup_rotation_alert() {
  PT_INIT(&pt_rotation_alert);
}

int schedule_rotation_alert(struct pt *pt) {
  static int slope[2];
  PT_BEGIN(pt);
  for(;;) {
    if (!rotation_alert_active) {
      PT_SLEEP(pt, 2000);
    }
    getMPUSlope(slope);
    if (abs(slope[0]) > 5 || abs(slope[1]) > 5) {
      rotation_alert_active = true;
      setLcdLine0Text("Tilt the box is not allowed");
      playNote(NOTE_F6);
      PT_SLEEP(pt, 100);
      stopBuzzer();
      PT_SLEEP(pt, 30);
    } else {
      if (rotation_alert_active) {
        rotation_alert_active = false;
        resetDefaultLcdText();
      }
    }
  }
  PT_END(pt);
}

void loop_rotation_alert() {
  PT_SCHEDULE(schedule_rotation_alert(&pt_rotation_alert));
}
