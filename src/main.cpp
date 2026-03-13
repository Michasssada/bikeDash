#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial Scooter(2);
float speed = 0;
#define RX_PIN 2

void setup()
{
  Serial.begin(115200);
  delay(500);

  Scooter.begin(115200, SERIAL_8N1, RX_PIN, -1);

  Serial.println("Xiaomi 1S telemetry sniffer started");
}

void loop()
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

        // Serial.print(valid ? "[OK] " : "[BAD] ");
        for (int i = 0; i < idx; i++)
          // Serial.printf("%02X ", buf[i]);
          // Serial.println();

          if (valid)
          {

            uint8_t addr = buf[3];
            uint8_t cmd = buf[4];

            // ESC reply
            if (addr == 0x21 && cmd == 0x64)
            {
              uint8_t *data = &buf[7];

              uint8_t battery = data[0];

              uint16_t rawSpeed = data[3] | (data[4] << 8);
              speed = rawSpeed;
              Serial.printf(
                  "SPEED %.2f km/h | BAT %d%%\n | Voltage %.2f V | Current %.2f A\n",
                  speed,
                  battery);
            }
          }

        idx = 0;
      }
    }

    if (idx >= sizeof(buf))
      idx = 0;
  }
}