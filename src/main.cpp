#ifdef ARDUINO
#include <Arduino.h>
#include "LGFX_Setup.h"
#include <lvgl.h>
#include "lv_conf.h"
#include "ui/ui.h"

// OTA & WiFi Includes
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

// --- CONFIGURATION ---
#define WIFI_SSID "YOUR_WIFI_SSID"     // <--- CHANGE THIS
#define WIFI_PASS "YOUR_WIFI_PASSWORD" // <--- CHANGE THIS
#define FIRMWARE_URL "https://raw.githubusercontent.com/Golabstech/VibeGo/master/firmware/firmware.bin"
#define APP_VERSION "1.0.0"

static LGFX lcd;

/* Change to your screen resolution */
#ifdef WOKWI
static const uint32_t screenWidth  = 240;
static const uint32_t screenHeight = 320;
#define MQ3_PIN 4 
#else
static const uint32_t screenWidth  = 800;  // WaveShare 4.3"
static const uint32_t screenHeight = 480;  // WaveShare 4.3"
#define MQ3_PIN 4 
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    lcd.startWrite();
    lcd.setAddrWindow( area->x1, area->y1, w, h );
    lcd.writePixels( (lgfx::rgb565_t *)&color_p->full, w * h );
    lcd.endWrite();

    lv_disp_flush_ready( disp );
}

/* Read the touchpad */
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch( &touchX, &touchY );

    if( touched )
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

// --- OTA FUNCTIONS ---
void connectWiFi() {
    Serial.print("Connecting to WiFi");
    lcd.fillScreen(0x0000);
    lcd.setTextColor(0xFFFF);
    lcd.setTextSize(2);
    lcd.setCursor(10, 10);
    lcd.println("Connecting to WiFi...");
    lcd.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) { // 10 seconds timeout
        delay(500);
        Serial.print(".");
        lcd.print(".");
        timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        lcd.println("\nConnected!");
        lcd.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi Connection Failed!");
        lcd.println("\nConnection Failed!");
        lcd.println("Skipping OTA...");
        delay(2000);
    }
}

void checkOTA() {
    if (WiFi.status() != WL_CONNECTED) return;

    Serial.println("Checking for Update...");
    lcd.println("Checking Update...");

    WiFiClientSecure client;
    client.setInsecure(); // Skip certificate validation for simplicity

    // Note: In a real scenario, you should check version.json first.
    // Here we try to update directly. HTTPUpdate handles "no update" if file is same? 
    // Actually, it just downloads whatever is there. 
    // For production, implement version check logic here.
    
    t_httpUpdate_return ret = httpUpdate.update(client, FIRMWARE_URL);
    
    switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            lcd.println("Update Failed");
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            lcd.println("No Updates");
            break;
        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            lcd.println("Update OK! Rebooting...");
            break;
    }
    
    lcd.println("OTA Check Done (Demo)");
    delay(1000);
}

// --- APP LOGIC ---
enum AppState {
    STATE_IDLE,
    STATE_MEASURING,
    STATE_RESULT
};

AppState appState = STATE_IDLE;
unsigned long measureStartTime = 0;
int maxAnalogValue = 0;
const int THRESHOLD_START = 1500; 
const int THRESHOLD_DANGER = 2000; 

void setup()
{
    Serial.begin( 115200 );
    Serial.println("Starting Setup...");

    pinMode(MQ3_PIN, INPUT);

    if (!lcd.init()) {
        Serial.println("LGFX Init Failed!");
    } else {
        Serial.println("LGFX Initialized");
    }
    
    lcd.setRotation( 0 );
    lcd.setBrightness(255);

    // 1. Connect WiFi & Check OTA
    #ifndef WOKWI // Skip WiFi in Wokwi for speed unless needed
    connectWiFi();
    checkOTA();
    #endif

    // 2. Init LVGL
    lv_init();
    Serial.println("LVGL Initialized");

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    ui_init();
    Serial.println("UI Initialized");
}

void loop()
{
    lv_timer_handler(); 
    
    int sensorValue = analogRead(MQ3_PIN);
    
    switch (appState) {
        case STATE_IDLE:
            if (lv_scr_act() == ui_Home) {
                if (sensorValue > THRESHOLD_START) {
                    Serial.println("Breath Detected! Starting Measurement...");
                    lv_scr_load_anim(ui_Measuring, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
                    appState = STATE_MEASURING;
                    measureStartTime = millis();
                    maxAnalogValue = 0;
                }
            }
            break;

        case STATE_MEASURING:
            if (sensorValue > maxAnalogValue) {
                maxAnalogValue = sensorValue;
            }
            if (millis() - measureStartTime > 3000) {
                Serial.printf("Measurement Done. Max Value: %d\n", maxAnalogValue);
                
                float promil = 0.0;
                if (maxAnalogValue > 1000) {
                    promil = (float)(maxAnalogValue - 1000) / 800.0; 
                }
                
                char promilStr[10];
                sprintf(promilStr, "%.2f", promil);

                if (promil >= 0.50) {
                    if (ui_Result_Danger_Value) lv_label_set_text(ui_Result_Danger_Value, promilStr);
                    lv_scr_load_anim(ui_Result_Danger, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
                } else {
                    if (ui_Result_Safe_Value) lv_label_set_text(ui_Result_Safe_Value, promilStr);
                    lv_scr_load_anim(ui_Result_Safe, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
                }
                
                appState = STATE_RESULT;
            }
            break;

        case STATE_RESULT:
            if (lv_scr_act() == ui_Disclaimer || lv_scr_act() == ui_Home) {
                appState = STATE_IDLE;
            }
            break;
    }

    delay( 5 );
}

#else // NATIVE / EMULATOR
// ... (Keep existing emulator code)
#include "lvgl.h"
#include "lv_drivers/sdl/sdl.h"
#include "ui/ui.h"
#include <unistd.h>
#include <time.h>

int main(void)
{
    lv_init();
    sdl_init();
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[480 * 10];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, 480 * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 272;
    lv_disp_drv_register(&disp_drv);
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = sdl_mouse_read;
    lv_indev_drv_register(&indev_drv_1);
    ui_init();
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
#endif
