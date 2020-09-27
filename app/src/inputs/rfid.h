#ifndef rfid_h
#define rfid_h

#include <Arduino.h>

//bool getRfidPresentCardUUID(byte presentCardUUID[4]);
bool isRfidPresentCardUUID(byte validCardUUID[4]);
bool isRfidCardPresent();
void setup_rfid();
void loop_rfid();

#endif