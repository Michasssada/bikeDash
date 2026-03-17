#include "getSteering.hpp"
steering::steering()
{
    throttle = 0;
    brake = 0;
    pinMode(3, PULLUP);
    pinMode(4, PULLUP);
}
void steering::update()
{
    throttle = map(constrain(int(analogRead(throttlePin) - 1000), 0, 2000), 0, 2000, 0, 255);
    brake = map(constrain(int(analogRead(brakePin) - 1000), 0, 2000), 0, 2000, 0, 255);
    Serial.println("Throttle: " + String(throttle) + " Brake: " + String(brake));
}
steering steer;