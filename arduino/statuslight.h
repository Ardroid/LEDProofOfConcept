#ifndef ARDROID_STATUSLIGHT_H
#define ARDROID_STATUSLIGHT_H

#include "Arduino.h"

#define CONNECTION_NOT_ESTABLISHED 0
#define CONNECTION_ESTABLISHED 1
#define BOT_IDLING 2
#define BOT_MOVING 3
#define flipmode(mode) (m_mode == LOW ? HIGH : LOW)

class StatusLight{
  byte m_pin;
  unsigned long m_lastActivated;
  byte m_mode;
  byte m_status;
  boolean m_running;

public:
  StatusLight() : m_pin(13), m_lastActivated(0), m_mode(LOW), m_status(CONNECTION_NOT_ESTABLISHED), m_running(false) {} // Many arduinos has this LED.
  StatusLight(byte pin) : m_lastActivated(0), m_mode(LOW), m_status(CONNECTION_NOT_ESTABLISHED), m_running(false) { m_pin = pin; }
  ~StatusLight() {}
  void start();
  void stop();
  void loop();
  void status(byte newStatus);
  byte status();
  byte getPin() { return m_pin; }
};

#endif
