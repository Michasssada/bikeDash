#include <TFT_eSPI.h>
#include <lvgl.h>
TFT_eSPI tft = TFT_eSPI();
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define DRAW_BUF_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
void begin()
{
    tft.init();
    lv_init();
    lv_display_t *disp;
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_180);
    disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
}
void update()
{
    lv_task_handler();
    lv_tick_inc(5); // tell LVGL how much time has passed
}