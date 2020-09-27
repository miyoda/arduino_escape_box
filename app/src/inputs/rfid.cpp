#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "rfid.h"

int const PIN_RFID_SS = 53;
int const PIN_RFID_RST = 49;

MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST); 

bool isRfidCardPresent() {
  return mfrc522.PICC_IsNewCardPresent();
}

bool isRfidPresentCardUUID(byte validCardUUID[4]) {
  if (mfrc522.PICC_ReadCardSerial()) {
    Serial.print(F("Present Card UID:"));
    bool equals = true;
    for (byte i = 0; i < 4; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      equals &= validCardUUID[i] != mfrc522.uid.uidByte[i];
    }
    Serial.println();
    mfrc522.PICC_HaltA();
    return equals;
  } else {
    return false;
  }
}

/*bool getRfidPresentCardUUID(byte presentCardUUID[4]) {
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
}*/

bool compareUUID(byte *array1,byte *array2) {
  return array1[0] == array2[0]
    && array1[1] == array2[1]
    && array1[2] == array2[2]
    && array1[3] == array2[3];
}

void setup_rfid() {
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop_rfid() {

}

