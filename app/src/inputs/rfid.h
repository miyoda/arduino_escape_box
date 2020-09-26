#ifndef rfid_h
#define rfid_h

#include <Arduino.h>

bool getRfidPresentCardUUID(byte presentCardUUID[4]);
void setup_rfid();
void loop_rfid();

#endif