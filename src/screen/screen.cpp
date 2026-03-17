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
}
