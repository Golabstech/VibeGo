/**
 * ============================================================
 * AlkoMetric / VibeGo Kiosk - Ana Program
 * Platform: JC2432W328 (ESP32 + ST7789 + CST820)
 * Display: 320x240 Landscape Mode
 * 
 * Features:
 * - WiFi with Captive Portal (WiFiManager)
 * - OTA Updates (ArduinoOTA)
 * - Auto Brightness (LDR on GPIO34)
 * - Sensor Simulation Mode
 * ============================================================
 */

#include <Arduino.h>
#include <lvgl.h>
#include "LGFX_Setup.h"
#include "ui/ui.h"
#include "wifi_handler.h"
#include "ota_handler.h"
#include "brightness.h"

// ============================================================
// Firmware Version
// ============================================================
#define FIRMWARE_VERSION "1.0.0"

// ============================================================
// Display Configuration (Landscape)
// ============================================================
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define DISPLAY_ROTATION 1  // 1 = Landscape

// ============================================================
// Sensor Simulation
// ============================================================
#define SENSOR_SIMULATION_ENABLED true
#define SIMULATION_DURATION_MS 4000
#define BAC_THRESHOLD 0.50f

// ============================================================
// Service Mode (Hidden Settings Access)
// ============================================================
#define SERVICE_MODE_TAP_COUNT 5       // Number of taps on logo
#define SERVICE_MODE_TAP_TIMEOUT 3000  // 3 seconds

static int service_tap_count = 0;
static uint32_t service_tap_start = 0;

// ============================================================
// Global Objects
// ============================================================
static LGFX lcd;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISPLAY_WIDTH * 20];  // Reduced from 40 to 20 lines

// Simulation state
static bool is_measuring = false;
static uint32_t measurement_start_time = 0;

// ============================================================
// Brightness Callback
// ============================================================
void set_display_brightness(int level) {
    lcd.setBrightness(level);
}

// ============================================================
// LVGL Display Flush Callback
// ============================================================
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    lcd.endWrite();

    lv_disp_flush_ready(disp_drv);
}

// ============================================================
// LVGL Touch Read Callback
// ============================================================
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    
    if (lcd.getTouch(&touchX, &touchY)) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ============================================================
// Sensor Simulation
// ============================================================
float generate_random_bac() {
    randomSeed(analogRead(0) + millis());
    int chance = random(100);
    
    if (chance < 70) {
        return (float)random(0, 50) / 100.0f;
    } else {
        return (float)random(50, 200) / 100.0f;
    }
}

void start_measurement_simulation() {
    if (!SENSOR_SIMULATION_ENABLED) return;
    is_measuring = true;
    measurement_start_time = millis();
    Serial.println("[SIM] Measurement started...");
}

void update_measurement_simulation() {
    if (!is_measuring) return;
    
    uint32_t elapsed = millis() - measurement_start_time;
    int progress = (elapsed * 100) / SIMULATION_DURATION_MS;
    if (progress > 100) progress = 100;
    
    extern lv_obj_t * ui_Measuring_Progress;
    if (ui_Measuring_Progress != NULL) {
        lv_bar_set_value(ui_Measuring_Progress, progress, LV_ANIM_ON);
    }
    
    if (elapsed >= SIMULATION_DURATION_MS) {
        is_measuring = false;
        float bac = generate_random_bac();
        Serial.printf("[SIM] BAC = %.2f\n", bac);
        
        if (bac < BAC_THRESHOLD) {
            ui_show_result_safe(bac);
        } else {
            ui_show_result_danger(bac);
        }
        
        lv_bar_set_value(ui_Measuring_Progress, 0, LV_ANIM_OFF);
    }
}

void screen_load_event_cb(lv_event_t * e) {
    lv_obj_t * scr = lv_event_get_target(e);
    extern lv_obj_t * ui_Measuring;
    if (scr == ui_Measuring) {
        start_measurement_simulation();
    }
}

// ============================================================
// Service Mode Detection (5 taps on VIBEGO logo)
// ============================================================
void check_service_mode(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    // Use PRESSED event (code 21) since CLICKED doesn't fire
    if (code != LV_EVENT_PRESSED) return;
    
    uint32_t now = millis();
    
    // Reset if timeout
    if (now - service_tap_start > SERVICE_MODE_TAP_TIMEOUT) {
        service_tap_count = 0;
    }
    
    // First tap
    if (service_tap_count == 0) {
        service_tap_start = now;
    }
    
    service_tap_count++;
    Serial.printf("[LOGO] Tap count: %d/%d\n", service_tap_count, SERVICE_MODE_TAP_COUNT);
    
    // Check if enough taps
    if (service_tap_count >= SERVICE_MODE_TAP_COUNT) {
        Serial.println("[SERVICE] Entering Service Mode!");
        service_tap_count = 0;
        
        // Update settings screen with current info
        ui_settings_update_wifi(wifi_get_status_str(), wifi_get_ip().c_str());
        ui_settings_update_brightness(brightness_get_level(), brightness_is_auto());
        
        // Show settings
        ui_settings_show();
    }
}

// ============================================================
// Serial Command Handler
// ============================================================
void handle_serial_commands() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        
        if (cmd == "settings" || cmd == "service") {
            Serial.println("[CMD] Opening Settings...");
            ui_settings_update_wifi(wifi_get_status_str(), wifi_get_ip().c_str());
            ui_settings_update_brightness(brightness_get_level(), brightness_is_auto());
            ui_settings_show();
        }
        else if (cmd == "wifi_reset") {
            Serial.println("[CMD] Resetting WiFi...");
            wifi_reset();
        }
        else if (cmd == "wifi_portal") {
            Serial.println("[CMD] Starting WiFi Portal...");
            wifi_start_portal();
        }
        else if (cmd == "reboot") {
            Serial.println("[CMD] Rebooting...");
            ESP.restart();
        }
        else if (cmd == "status") {
            Serial.println("=== STATUS ===");
            Serial.printf("WiFi: %s\n", wifi_get_status_str());
            Serial.printf("IP: %s\n", wifi_get_ip().c_str());
            Serial.printf("SSID: %s\n", wifi_get_ssid().c_str());
            Serial.printf("Signal: %d%%\n", wifi_get_signal_percent());
            Serial.printf("Brightness: %d (Auto: %s)\n", brightness_get_level(), brightness_is_auto() ? "Yes" : "No");
            Serial.printf("LDR Raw: %d\n", brightness_get_ldr_raw());
            Serial.printf("Firmware: v%s\n", FIRMWARE_VERSION);
        }
        else if (cmd == "help") {
            Serial.println("=== COMMANDS ===");
            Serial.println("settings   - Open settings screen");
            Serial.println("wifi_reset - Reset WiFi credentials");
            Serial.println("wifi_portal- Start WiFi config portal");
            Serial.println("reboot     - Restart device");
            Serial.println("status     - Show system status");
        }
    }
}

// ============================================================
// Setup
// ============================================================
void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println();
    Serial.println("╔════════════════════════════════════════╗");
    Serial.println("║      VIBEGO - AlkoMetric Kiosk         ║");
    Serial.printf("║      Firmware: v%s                   ║\n", FIRMWARE_VERSION);
    Serial.println("║      Platform: JC2432W328              ║");
    Serial.println("╚════════════════════════════════════════╝");
    Serial.println();

    // Initialize Display
    Serial.print("[INIT] Display...");
    lcd.init();
    lcd.setRotation(DISPLAY_ROTATION);
    lcd.setBrightness(255);
    lcd.fillScreen(TFT_BLACK);
    Serial.println(" OK");

    // Initialize Brightness (LDR)
    Serial.print("[INIT] LDR Brightness...");
    brightness_init();
    brightness_set_callback(set_display_brightness);
    Serial.println(" OK");

    // Initialize LVGL
    Serial.print("[INIT] LVGL...");
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISPLAY_WIDTH * 20);
    
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println(" OK");
    
    // Initialize Touch
    Serial.print("[INIT] Touch...");
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    Serial.println(" OK");

    // Initialize UI
    Serial.print("[INIT] UI...");
    ui_init();
    Serial.println(" OK");

    // Setup screen event for simulation
    extern lv_obj_t * ui_Measuring;
    lv_obj_add_event_cb(ui_Measuring, screen_load_event_cb, LV_EVENT_SCREEN_LOADED, NULL);
    
    // Setup service mode detection on logo
    extern lv_obj_t * ui_Home_Logo;
    lv_obj_add_flag(ui_Home_Logo, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Home_Logo, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(ui_Home_Logo, check_service_mode, LV_EVENT_ALL, NULL);
    Serial.println("[INIT] Logo tap detection ready");

    // Initialize WiFi - Start AP if no saved credentials
    Serial.println("[INIT] WiFi...");
    if (!wifi_init(false)) {
        // No saved WiFi, start config portal in background
        Serial.println("[WIFI] No saved network, starting AP...");
        // Note: Portal will start when user requests via serial
    }
    
    // LDR Debug
    Serial.printf("[INIT] LDR Raw Value: %d\n", brightness_get_ldr_raw());

    Serial.println();
    Serial.println("[READY] System started!");
    Serial.println("[INFO] Type 'help' for commands");
    Serial.println();
}

// ============================================================
// Main Loop
// ============================================================
void loop() {
    // LVGL
    lv_timer_handler();
    
    // WiFi status update
    wifi_update();
    
    // OTA (init when WiFi connected)
    static bool ota_started = false;
    if (wifi_get_status() == WIFI_STATUS_CONNECTED && !ota_started) {
        ota_init();
        ota_started = true;
    }
    ota_handle();
    
    // Brightness (auto)
    brightness_update();
    
    // Sensor simulation
    if (SENSOR_SIMULATION_ENABLED) {
        update_measurement_simulation();
    }
    
    // Serial commands
    handle_serial_commands();
    
    delay(5);
}
