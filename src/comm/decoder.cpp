#include "decoder.hpp"
float speed = 0;
float battery = 0;
uint8_t lights = 0;
uint8_t modeFlags = 0;
uint8_t throttle = 0;
uint8_t brake = 0;
uint8_t speedMode = 0;
HardwareSerial Scooter(2);
// example packet:
//  [OK] 55 AA 07 20 65 00|04 27 22 00 00|26 FF
//  [OK] 55 AA 07 20 65 00|04 27 22 00 00|26 FF
//  [OK] 55 AA 07 20 65 00|04 27 22 00 00|26 FF
//  [OK] 55 AA 07 20 65 00|04 27 22 00 00|26 FF
//  [OK] 55 AA 09 20 64 00|06 27 22 00 00 57 01|CB FE
//  [OK] 55 AA 08 21 64 00|02 3E 00 00 00 00|32 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 09 20 64 00 06 27 22 00 00 57 01 CB FE
// [OK] 55 AA 08 21 64 00 02 4A 00 00 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 09 20 64 00 06 27 22 00 00 57 01 CB FE
// [OK] 55 AA 08 21 64 00 00 4A 00 00 00 00 28 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 07 20 65 00 04 27 22 00 00 26 FF
// [OK] 55 AA 09 20 64 00 06 27 22 00 00 57 01 CB FE
// [OK] 55 AA 08 21 64 00 04 4A 00 00 00 00 24 FF
void begin()
{

    Serial.begin(115200);
    Scooter.begin(115200, SERIAL_8N1, RX_PIN, -1);
}
void update()
{
    static uint8_t buf[96];
    static int idx = 0;

    while (Scooter.available())
    {

        uint8_t b = Scooter.read();

        // wait for first header byte
        if (idx == 0 && b != 0x55)
            continue;

        buf[idx++] = b;

        // header check
        if (idx >= 2 && (buf[0] != 0x55 || buf[1] != 0xAA))
        {
            idx = 0;
            continue;
        }

        if (idx >= 6)
        {

            uint8_t len = buf[2];
            int expected = 4 + len + 2;

            if (idx >= expected)
            {

                // checksum
                uint16_t sum = 0;
                for (int i = 2; i < idx - 2; i++)
                    sum += buf[i];

                uint16_t calc = ~sum;
                uint16_t rx = buf[idx - 2] | (buf[idx - 1] << 8);

                bool valid = (calc == rx);

                Serial.print(valid ? "[OK] " : "[BAD] ");
                for (int i = 0; i < idx; i++)
                    //     Serial.printf("%02X ", buf[i]);
                    // Serial.println();

                    if (valid)
                    {

                        uint8_t addr = buf[3];
                        uint8_t cmd = buf[4];

                        // ESC reply
                        if (addr == 0x21 && cmd == 0x64)
                        {
                            uint8_t *data = &buf[6];
                            speedMode = data[0];
                            battery = data[1];
                            uint16_t rawSpeed = data[4] | (data[5] << 8);
                            speed = rawSpeed;
                            lights = data[2];
                            // Serial.printf(
                            Serial.println(data[0]);
                            //     "SPEED %.2f km/h | BAT %d%%\n, test: %d",
                            //     speed,
                            //     (int)battery, (int)data[1]);
                        }
                        if (addr == 0x20 && cmd == 0x65)
                        {
                            uint8_t *data = &buf[6];
                            throttle = data[1];
                            brake = data[2];
                            Serial.println(data[3]);
                        }
                        if (addr == 0x20 && cmd == 0x64)
                        {
                            uint8_t *data = &buf[7];
                        }
                    }

                idx = 0;
            }
        }

        if (idx >= sizeof(buf))
            idx = 0;
    }
}