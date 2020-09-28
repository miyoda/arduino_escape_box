#include <protothreads.h>
#include <Arduino.h>
#include <MFRC522.h>
#include "rfid_identify_card.h"
#include "../game.h"
#include "../inputs/rfid.h"
#include "../outputs/buzzer.h"
#include "../outputs/led_array.h"

static struct pt pt_rfid_identify_card;
static bool rfid_identify_card_passed = false;

static byte whiteCardUUID[4] = {0x69, 0x6C, 0x58, 0xB3};

void setup_rfid_identify_card() {
  PT_INIT(&pt_rfid_identify_card);
}

bool is_rfid_identify_card_passed() {
  return rfid_identify_card_passed;
}

int schedule_rfid_identify_card_finish_game(struct pt *pt) {
  PT_BEGIN(pt);
  PT_SLEEP(pt, 3000);
  for(;;) {
    PT_SLEEP(pt, 1000);
    if (isRfidCardPresent()) {
      if(isRfidPresentCardUUID(whiteCardUUID)) {
        playMelody(MELODY_SUCCESS);
        rfid_identify_card_passed = true;
      } else {
        playMelody(MELODY_FAILURE);
      }
    }
  }
  PT_END(pt);
}

void loop_rfid_identify_card() {
  if (!rfid_identify_card_passed) {
    PT_SCHEDULE(schedule_rfid_identify_card_finish_game(&pt_rfid_identify_card));
  }
}
