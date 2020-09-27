#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rfid_identify_keychain.h"
#include "../game.h"
#include "../inputs/rfid.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"

static struct pt pt_rfid_identify_keychain_finish_game;
static bool rfid_identify_keychain_passed = false;

//static byte whiteCardUUID[4] = {0x69, 0x6C, 0x58, 0xB3}; // Lavero
static byte keychainCardUUID[4] = {0x43, 0x46, 0xE2, 0x39}; // Tarjeta

void setup_rfid_identify_keychain() {
  PT_INIT(&pt_rfid_identify_keychain_finish_game);
}

bool is_rfid_identify_keychain_passed() {
  return rfid_identify_keychain_passed;
}

int schedule_rfid_identify_keychain_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  PT_SLEEP(pt, 3000);
  for(;;) {
    PT_SLEEP(pt, 1000);
    if (isRfidCardPresent()) {
      if(isRfidPresentCardUUID(keychainCardUUID)) {
        playMelody(MELODY_SUCCESS);
        rfid_identify_keychain_passed = true;
      } else {
        playMelody(MELODY_FAILURE);
      }
    }
  }
  PT_END(pt);
}

void loop_rfid_identify_keychain() {
  if (!rfid_identify_keychain_passed) {
    PT_SCHEDULE(schedule_rfid_identify_keychain_finish_game(&pt_rfid_identify_keychain_finish_game));
  }
}
