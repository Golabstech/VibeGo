#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>

// Extended IO Pin definitions for Waveshare ESP32-S3-Touch-LCD-5B if needed
// The library handles most via ESP_Panel_Conf.h, but we ensure correct init here.

ESP_Panel *panel = nullptr;

// LVGL Drawing Buffer
#define LCD_WIDTH   1024
#define LCD_HEIGHT  600
#define LVGL_BUF_SIZE (LCD_WIDTH * 20) 

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf = nullptr;

// LVGL Flush Callback
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    if (panel->getLcd()) {
        panel->getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    }
    lv_disp_flush_ready(disp_drv);
}

// LVGL Touch Callback
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    if (panel->getLcdTouch()) {
        panel->getLcdTouch()->readData();
        bool touched = panel->getLcdTouch()->getTouchState();
        
        if (touched) {
            TouchPoint point = panel->getLcdTouch()->getPoint(0);
            data->state = LV_INDEV_STATE_PRESSED;
            data->point.x = point.x;
            data->point.y = point.y;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Waveshare ESP32-S3-Touch-LCD-5B Test");

    // Initialize Panel
    panel = new ESP_Panel();
    panel->init();
    panel->begin();
    
    // Initialize LVGL
    lv_init();
    
    // Allocate buffer
    buf = (lv_color_t *)heap_caps_malloc(LVGL_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    if (!buf) {
        Serial.println("DMA Alloc Failed, trying generic malloc");
        buf = (lv_color_t *)malloc(LVGL_BUF_SIZE * sizeof(lv_color_t));
    }
    
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_BUF_SIZE);

    // Initialize Display Driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize Touch Driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Create Test UI
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Waveshare Display Test\nESP32-S3 RGB 1024x600");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    // Add a button to test touch
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Touch Me");

    Serial.println("Setup Done");
}

void loop() {
    lv_timer_handler();
    delay(5);
}
