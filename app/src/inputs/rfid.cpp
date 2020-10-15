#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "rfid.h"

/*
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

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

