#include "screen.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 15 // Keep this — good choice
#define TFT_MOSI 11
#define TFT_SCLK 12
float old_speed = 0;
uint16_t old_color = 0;
// Use hardware SPI (ESP32 default pins ignored since we specify MOSI/SCK)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
int16_t getCenteredX(Adafruit_ST7735 &tft, const String &text, uint8_t textSize = 1)
{
    int16_t x1, y1;
    uint16_t w, h;

    tft.setTextSize(textSize);
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    return (tft.width() - w) / 2;
}

// Returns the Y coordinate to vertically center the given text
int16_t getCenteredY(Adafruit_ST7735 &tft, const String &text, uint8_t textSize = 1)
{
    int16_t x1, y1;
    uint16_t w, h;

    tft.setTextSize(textSize);
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    return (tft.height() - h) / 2;
}
void updateText(Adafruit_GFX &tft, int16_t x, int16_t y, String newText, uint16_t textColor = ST7735_WHITE, uint16_t bgColor = ST7735_BLACK)
{
    static String oldText = ""; // stores previous text
    if (oldText != newText)
    { // only update if different
        tft.setCursor(x, y);
        tft.setTextColor(textColor, bgColor); // text color + background erase
        tft.print(newText);
        oldText = newText;
    }
}
void lcd_init()
{
    tft.initR(INITR_GREENTAB); // or GREENTAB / REDTAB — test variants
    tft.setRotation(2);
    tft.fillScreen(ST7735_BLACK);
}
void lcd_update()
{
    uint8_t r = map(battery, 0, 100, 255, 0);
    uint8_t g = map(battery, 0, 100, 0, 255);
    uint8_t b = 0; // keep blue off

    // convert to 16-bit color
    uint16_t color = tft.color565(b, g, r);
    tft.setTextSize(3);
    String speedText = String(trunc(speed), 0);
    updateText(tft, getCenteredX(tft, speedText, 3), getCenteredY(tft, speedText, 3), speedText);
    String batteryText = String(trunc(battery), 0) + "%";
    tft.setTextSize(2);
    if (color != old_color)
    {
        tft.fillRect(0, tft.height() - 40, tft.width(), 40, color);
        old_color = color;
    }
    if (lights == 1)
    {
        analogWrite(38, 150);
        tft.fillRect(0, 0, tft.width(), 20, ST7735_WHITE);
    }
    else
    {
        analogWrite(38, 255);
        tft.fillRect(0, 0, tft.width(), 20, ST7735_BLACK);
    }
    updateText(tft, getCenteredX(tft, batteryText, 2), 130, batteryText, ST7735_WHITE, color);
    tft.setTextSize(1);
    String speedModeText;
    uint16_t modeColor;
    switch (speedMode)
    {
    case 0:
        speedModeText = "Normal";
        modeColor = ST7735_BLUE;
        break;

    case 2:
        speedModeText = "Eco   ";
        modeColor = ST7735_GREEN;
        break;

    case 4:
        speedModeText = "Sport ";
        modeColor = ST7735_RED;
        break;
    }
    updateText(tft, getCenteredX(tft, speedModeText, 1), 30, speedModeText, modeColor, ST7735_BLACK);
}
