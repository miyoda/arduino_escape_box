#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "rfid.h"

int const PIN_RFID_SS = 53;
int const PIN_RFID_RST = 49;

MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST); 

bool getRfidPresentCardUUID(byte presentCardUUID[4]) {
  if (mfrc522.PICC_IsNewCardPresent()) {  
  	if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        presentCardUUID[i] = mfrc522.uid.uidByte[i];
      }
      Serial.println();
      mfrc522.PICC_HaltA();
      return true;
    }
	}
  return false;
}

void setup_rfid() {
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop_rfid() {

}

