#pragma once
#include "Arduino.h"
extern float speed;
extern float battery;
extern uint8_t modeFlags;
extern uint8_t lights;
extern uint8_t throttle;
extern uint8_t brake;
extern uint8_t speedMode;
#define RX_PIN 1
void begin();
void update();