/**
 * ============================================================
 * VibeGo - OTA Update Handler
 * ============================================================
 * ArduinoOTA for wireless firmware updates
 * ============================================================
 */

#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

// ============================================================
// Configuration
// ============================================================
#define OTA_HOSTNAME   "vibego"
#define OTA_PASSWORD   ""           // Empty = no password
#define OTA_PORT       3232

// ============================================================
// State
// ============================================================
static bool ota_initialized = false;
static bool ota_in_progress = false;
static int ota_progress = 0;

// ============================================================
// Callbacks (can be set by main app)
// ============================================================
typedef void (*ota_progress_cb_t)(int percent);
static ota_progress_cb_t ota_progress_callback = nullptr;

// ============================================================
// Functions
// ============================================================

/**
 * Set progress callback
 */
void ota_set_progress_callback(ota_progress_cb_t cb) {
    ota_progress_callback = cb;
}

/**
 * Initialize OTA
 * Call after WiFi is connected
 */
void ota_init() {
    if (ota_initialized) return;
    if (!WiFi.isConnected()) {
        Serial.println("[OTA] WiFi not connected, skipping init");
        return;
    }
    
    Serial.println("[OTA] Initializing...");
    
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPort(OTA_PORT);
    
    if (strlen(OTA_PASSWORD) > 0) {
        ArduinoOTA.setPassword(OTA_PASSWORD);
    }
    
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "firmware";
        } else {
            type = "filesystem";
        }
        Serial.println("[OTA] Start updating " + type);
        ota_in_progress = true;
        ota_progress = 0;
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] Update complete!");
        ota_in_progress = false;
        ota_progress = 100;
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        int percent = (progress / (total / 100));
        if (percent != ota_progress) {
            ota_progress = percent;
            Serial.printf("[OTA] Progress: %u%%\r", percent);
            
            if (ota_progress_callback) {
                ota_progress_callback(percent);
            }
        }
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("[OTA] Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        ota_in_progress = false;
    });
    
    ArduinoOTA.begin();
    ota_initialized = true;
    
    Serial.print("[OTA] Ready! Hostname: ");
    Serial.print(OTA_HOSTNAME);
    Serial.println(".local");
}

/**
 * Handle OTA (call in loop)
 */
void ota_handle() {
    if (!ota_initialized) return;
    if (!WiFi.isConnected()) return;
    
    ArduinoOTA.handle();
}

/**
 * Check if OTA update is in progress
 */
bool ota_is_updating() {
    return ota_in_progress;
}

/**
 * Get current OTA progress (0-100)
 */
int ota_get_progress() {
    return ota_progress;
}

#endif // OTA_HANDLER_H
