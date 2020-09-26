#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rfid_identify.h"
#include "../game.h"
#include "../inputs/rfid.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"

static struct pt pt_rfid_identify_finish_game;
static bool rfid_identify_passed = false;

static byte presentCardUUID[4] = {0x00, 0x00, 0x00, 0x00};
static byte validCardUUID[4] = {0x43, 0x46, 0xE2, 0x39}; // Tarjeta
// static byte validCardUUID[4] = {0x69, 0x6C, 0x58, 0xB3}; // Lavero

void setup_rfid_identify() {
  PT_INIT(&pt_rfid_identify_finish_game);
}

bool is_rfid_identify_passed() {
  return rfid_identify_passed;
}

bool compareUUID(byte *array1,byte *array2) {
  return array1[0] == array2[0]
    && array1[1] == array2[1]
    && array1[2] == array2[2]
    && array1[3] == array2[3];
}


int schedule_rfid_identify_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  PT_SLEEP(pt, 3000);
  for(;;) {
    PT_SLEEP(pt, 1000);
    bool present = getRfidPresentCardUUID(presentCardUUID);
    if (present) {
      if(compareUUID(presentCardUUID, validCardUUID)) {
        playMelody(MELODY_SUCCESS);
        led_array_set(1, HIGH);
        rfid_identify_passed = true;
      } else {
        playMelody(MELODY_FAILURE);
      }
    }
  }
  PT_END(pt);
}

void loop_rfid_identify() {
  if (!rfid_identify_passed) {
    PT_SCHEDULE(schedule_rfid_identify_finish_game(&pt_rfid_identify_finish_game));
  }
}
