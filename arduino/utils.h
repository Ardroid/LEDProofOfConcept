#ifndef ARDROID_UTILS_H
#define ARDROID_UTILS_H

#include "Arduino.h"
#include "Adb.h"

typedef struct{
  uint8_t command;
  uint32_t timestamp;
  uint16_t length;
  uint8_t * data;
} Data;

uint32_t construct32bitInteger(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
uint8_t * destruct32bitInteger(uint32_t i);
Data * createData(const uint8_t *, uint16_t);
void destroyData(Data *);
bool sendData(Connection *, Data *);


#endif
