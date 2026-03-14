#include <Arduino.h>
#include <HardwareSerial.h>
#include "comm/decoder.hpp"
#include "screen/screen.hpp"
void setup()
{
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
  Serial.println("Xiaomi 1S telemetry sniffer started");
  begin();
  lcd_init();
}

void loop()
{
  lcd_update();
  update();
}