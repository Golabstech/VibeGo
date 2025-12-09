/**
 * ============================================================
 * AlkoMetric / VibeGo Kiosk - Ana Program
 * Platform: JC2432W328 (ESP32 + ST7789 + CST820)
 * Display: 320x240 Landscape Mode
 * 
 * Features:
 * - WiFi AP+STA Mode (Always accessible via AP)
 * - Web Dashboard for all settings
 * - HTTP OTA Updates
 * - Auto Brightness (LDR on GPIO34)
 * - Sensor Simulation Mode
 * ============================================================
 */

#include <Arduino.h>
#include <lvgl.h>
#include <esp_task_wdt.h>
#include "LGFX_Setup.h"
#include "ui/ui.h"
#include "wifi_handler.h"
#include "http_ota.h"
#include "brightness.h"
#include "ntp_time.h"
#include "data_logger.h"
#include "web_server.h"
#include "webhook_logger.h"
#include "sponsor_manager.h"

// ============================================================
// Firmware Version
// ============================================================
#define FIRMWARE_VERSION "1.3.0"

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
// Global Objects
// ============================================================
static LGFX lcd;
static lv_disp_draw_buf_t draw_buf;

// Double buffering for smoother rendering (DMA can write one while CPU fills other)
// Buffer size: 320 * 24 * 2 bytes * 2 buffers = ~30KB
#define DRAW_BUF_LINES 24  // 1/10 of screen height
static lv_color_t buf1[DISPLAY_WIDTH * DRAW_BUF_LINES];
static lv_color_t buf2[DISPLAY_WIDTH * DRAW_BUF_LINES];

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
        bool is_safe = bac < BAC_THRESHOLD;
        
        Serial.printf("[SIM] BAC = %.2f (%s)\n", bac, is_safe ? "Safe" : "Danger");
        
        // Log the test result
        logger_add_test(bac, is_safe);
        
        if (is_safe) {
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
// Serial Command Handler
// ============================================================
void handle_serial_commands() {
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        
        if (cmd == "wifi_reset") {
            Serial.println("[CMD] Resetting WiFi...");
            wifi_reset();
        }
        else if (cmd == "wifi_scan") {
            Serial.println("[CMD] Scanning networks...");
            int count = wifi_scan_networks();
            Serial.printf("[CMD] Found %d networks\n", count);
        }
        else if (cmd == "wifi_ap") {
            Serial.println("[CMD] Restarting AP...");
            wifi_restart_ap();
        }
        else if (cmd == "reboot") {
            Serial.println("[CMD] Rebooting...");
            ESP.restart();
        }
        else if (cmd == "status") {
            Serial.println("=== STATUS ===");
            Serial.printf("Firmware: v%s\n", http_ota_get_version());
            Serial.printf("AP Active: %s\n", wifi_is_ap_active() ? "Yes" : "No");
            if (wifi_is_ap_active()) {
                Serial.printf("AP IP: %s\n", wifi_get_ap_ip().c_str());
                int remaining = wifi_get_ap_remaining_seconds();
                if (remaining > 0) {
                    Serial.printf("AP Remaining: %d min %d sec\n", remaining / 60, remaining % 60);
                } else {
                    Serial.printf("AP Remaining: Unlimited (waiting for connection)\n");
                }
            }
            Serial.printf("STA Status: %s\n", wifi_get_status_str());
            if (wifi_is_connected()) {
                Serial.printf("STA SSID: %s\n", wifi_get_ssid().c_str());
                Serial.printf("STA IP: %s\n", wifi_get_sta_ip().c_str());
                Serial.printf("Signal: %d%%\n", wifi_get_signal_percent());
            }
            Serial.printf("Brightness: %d (Auto: %s)\n", brightness_get_level(), brightness_is_auto() ? "Yes" : "No");
            Serial.printf("LDR Raw: %d\n", brightness_get_ldr_raw());
        }
        else if (cmd == "update" || cmd == "ota") {
            Serial.println("[CMD] Checking for updates...");
            http_ota_force_update();
        }
        else if (cmd == "version") {
            Serial.printf("Firmware: v%s\n", http_ota_get_version());
        }
        else if (cmd == "help") {
            Serial.println("=== COMMANDS ===");
            Serial.println("status      - Show system status");
            Serial.println("update/ota  - Check and install updates");
            Serial.println("version     - Show firmware version");
            Serial.println("wifi_scan   - Scan WiFi networks");
            Serial.println("wifi_ap     - Restart AP mode (30 min)");
            Serial.println("wifi_reset  - Reset WiFi credentials");
            Serial.println("reboot      - Restart device");
            Serial.println("dashboard   - Show web panel URL");
        }
        else if (cmd == "dashboard") {
            Serial.printf("AP Dashboard: http://%s/\n", wifi_get_ap_ip().c_str());
            if (wifi_is_connected()) {
                Serial.printf("STA Dashboard: http://%s/\n", wifi_get_sta_ip().c_str());
            }
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
    Serial.printf("║      Firmware: v%s                  ║\n", FIRMWARE_VERSION);
    Serial.println("║      Platform: JC2432W328              ║");
    Serial.println("║      Mode: AP + STA                    ║");
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
    
    // Double buffering: DMA writes buf1 while CPU fills buf2, then swap
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISPLAY_WIDTH * DRAW_BUF_LINES);
    
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

    // Initialize WiFi (AP+STA mode)
    Serial.println("[INIT] WiFi AP+STA...");
    wifi_init(true);
    
    // Initialize NTP (if connected to external network)
    if (wifi_is_connected()) {
        ntp_init();
    }
    
    // Initialize Data Logger (SPIFFS)
    Serial.print("[INIT] Logger...");
    if (logger_init()) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    // Initialize Web Server (always start - works on AP)
    web_server_init();
    
    // Load saved sponsor QR URL
    sponsor_init();
    String savedQrUrl = sponsor_get_qr_url();
    String savedQrTitle = sponsor_get_qr_title();
    ui_update_qr_code(savedQrUrl.c_str());
    ui_update_qr_title(savedQrTitle.c_str());
    Serial.printf("[INIT] Sponsor: %s (%s)\n", savedQrTitle.c_str(), savedQrUrl.c_str());
    
    // Initialize Webhook (if connected)
    if (wifi_is_connected()) {
        webhook_init();
    }
    
    // Initialize Watchdog Timer (30 seconds)
    Serial.print("[INIT] Watchdog...");
    esp_task_wdt_init(30, true);
    esp_task_wdt_add(NULL);
    Serial.println(" OK (30s)");
    
    // LDR Debug
    Serial.printf("[INIT] LDR Raw Value: %d\n", brightness_get_ldr_raw());

    Serial.println();
    Serial.println("[READY] System started!");
    Serial.println("[INFO] Type 'help' for commands");
    Serial.printf("[WEB] AP Dashboard: http://%s/\n", wifi_get_ap_ip().c_str());
    if (wifi_is_connected()) {
        Serial.printf("[WEB] STA Dashboard: http://%s/\n", wifi_get_sta_ip().c_str());
    }
    Serial.println();
}

// ============================================================
// Main Loop
// ============================================================
void loop() {
    // Reset Watchdog Timer
    esp_task_wdt_reset();
    
    // LVGL
    lv_timer_handler();
    
    // WiFi status update
    wifi_update();
    
    // Delayed init: Start services when WiFi connects to external network
    static bool sta_services_started = false;
    if (!sta_services_started && wifi_is_connected()) {
        ntp_init();
        webhook_init();
        sta_services_started = true;
        Serial.println("[INIT] STA services started (NTP, Webhook)");
    }
    
    // NTP sync update (non-blocking)
    ntp_update();
    
    // HTTP OTA (GitHub - check every hour, only if connected to internet)
    if (wifi_is_connected()) {
        http_ota_check_periodic();
    }
    
    // Brightness (auto LDR)
    brightness_update();
    
    // Webhook queue sync check (only if connected)
    if (wifi_is_connected()) {
        webhook_update();
    }
    
    // Sensor simulation
    if (SENSOR_SIMULATION_ENABLED) {
        update_measurement_simulation();
    }
    
    // Serial commands
    handle_serial_commands();
    
    delay(5);
}
