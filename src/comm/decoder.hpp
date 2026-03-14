#pragma once
#include "Arduino.h"
extern float speed;
extern float battery;
extern uint8_t modeFlags;
extern uint8_t lights;
#define RX_PIN 1
void begin();
void update();