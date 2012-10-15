#include "utils.h"

uint32_t construct32bitInteger(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
  uint32_t g, h, j, k;
  g = a;
  h = b;
  j = c;
  k = d;
  return (g << 24) | (h << 16) | (j << 8) | k;
}

uint8_t * destruct32bitInteger(uint32_t i){
  uint8_t * t = (uint8_t*)malloc(sizeof(uint8_t)*4);
  t[3] = i;
  t[2] = i >> 8;
  t[1] = i >> 16;
  t[0] = i >> 24;
  return t;
}

Data * createData(const uint8_t * data, uint16_t length){
  if (length < 5) return NULL;
  length -= 5;
  Data * dataobj = (Data*) malloc(sizeof(Data));
  dataobj->command = data[0];
  dataobj->timestamp = construct32bitInteger(data[1], data[2], data[3], data[4]);
  dataobj->length = length;
  dataobj->data = (uint8_t*)malloc(sizeof(uint8_t)*length);
  uint16_t i;
  for (i=0; i<length; i++) dataobj->data[i] = data[i+5];
  return dataobj;
}

void destroyData(Data * data){
  free(data->data);
  free(data);
  data = NULL;
}

bool sendData(Connection * conn, Data * data){
  uint16_t length = data->length + 5;
  uint8_t * dataToSend = (uint8_t*)malloc(sizeof(uint8_t) * (length));
  dataToSend[0] = data->command;
  uint8_t * timestampBytes = destruct32bitInteger(data->timestamp);
  dataToSend[1] = timestampBytes[0];
  dataToSend[2] = timestampBytes[1];
  dataToSend[3] = timestampBytes[2];
  dataToSend[4] = timestampBytes[3];
  int i;
  for (i=5;i<length;i++) dataToSend[i] = data->data[i-5];
  free(timestampBytes);
  timestampBytes = NULL;
  return (conn->write(length, dataToSend) == 0);
}
