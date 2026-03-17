#include <Arduino.h>
#include <HardwareSerial.h>
#include "screen/screen.hpp"
#include "steering/getSteering.hpp"
void setup()
{
  Serial.begin(115200);
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
  lcd_init();
}

void loop()
{
  lcd_update();
  steer.update();
}