#include "statuslight.h"
#include <SPI.h>
#include "Adb.h"
#include "utils.h"

#define TIMEOUT 5000

// Apparently you have to do include <SPI.h> here for including it anywhere else to work.
// SPI is included in microbridge

StatusLight statusLight;
Connection *connection;
unsigned long lastPacketTime;

void idle(uint16_t length, Data * data, StatusLight statusLight){
  statusLight.status(BOT_IDLING);
}

void adbEventHandler(Connection * connection,
                     adb_eventType event,
                     uint16_t length,
                     uint8_t * data){

  if (event == ADB_CONNECTION_RECEIVE){
    if (length == 0){
      Serial.println("Empty data received. Ignored.");
    } else {
      Data * dobj = createData(data, length);
      switch (data[0]){
        case BOT_IDLING:
          idle(length, dobj, statusLight);
        break;
        default:
          statusLight.status(data[0]);
          Serial.println("Command not recognized.");
      }
      lastPacketTime = millis();
      destroyData(dobj);
    }
  }

}

void setup(){
  Serial.begin(9600);
  statusLight.start();
  ADB::init();
  connection = ADB::addConnection("tcp:4567", true, adbEventHandler);
  lastPacketTime = millis();
}

short x = 0;

void loop(){
  x += 1;
  statusLight.loop();

  if (millis() - lastPacketTime > TIMEOUT){
    if (connection->isOpen()){
      statusLight.status(CONNECTION_ESTABLISHED);
    } else {
      statusLight.status(CONNECTION_NOT_ESTABLISHED);
    }
  }

  ADB::poll();
}

