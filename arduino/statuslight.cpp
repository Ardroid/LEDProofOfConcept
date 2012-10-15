#include "statuslight.h"

void StatusLight::start(){
  m_running = true;
  pinMode(m_pin, OUTPUT);
}

void StatusLight::stop(){
  m_running = false;
  digitalWrite(m_pin, LOW);
  m_lastActivated = 0;
  m_mode = LOW;
  m_status = CONNECTION_NOT_ESTABLISHED;
}

void StatusLight::loop(){
  if (m_running){
    unsigned long currentTime = millis();
    unsigned int interval;
    switch (m_status){
      case CONNECTION_NOT_ESTABLISHED:
        interval = 200;
      break;
      case CONNECTION_ESTABLISHED:
        interval = 1000;
      break;
      case BOT_IDLING:
        interval = (m_mode == HIGH ? 200 : 1000); // If the mode is high, then the interval is 1s, otherwise it's 0.2s
      break;
      case BOT_MOVING:
        interval = (m_mode == HIGH ? 1000 : 0);
      break;
    }
    if (currentTime - m_lastActivated > interval){
      m_mode = flipmode(m_mode);
      digitalWrite(m_pin, m_mode);
      m_lastActivated = currentTime;
    }
  }
}

void StatusLight::status(byte newStatus){
  m_status = newStatus;
}

byte StatusLight::status(){
  return m_status;
}
