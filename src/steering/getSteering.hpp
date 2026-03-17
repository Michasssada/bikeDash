#include <Arduino.h>
#define throttlePin 3
#define brakePin 4
class steering{
    public:
    uint8_t throttle;
    uint8_t brake;
    steering();
    void update();
};
extern steering steer;