/**
 * ============================================================
 * VibeGo - HTTP OTA Update Handler
 * ============================================================
 * Downloads firmware from GitHub when new version available
 * ============================================================
 */

#ifndef HTTP_OTA_H
#define HTTP_OTA_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

// ============================================================
// Configuration
// ============================================================
#define CURRENT_VERSION     "1.0.0"
#define VERSION_CHECK_URL   "https://raw.githubusercontent.com/Golabstech/VibeGo/master/firmware/version.json"
#define CHECK_INTERVAL_MS   (60 * 60 * 1000)  // Check every 1 hour

// ============================================================
// State
// ============================================================
static bool http_ota_updating = false;
static int http_ota_progress = 0;
static String new_version = "";
static String firmware_url = "";
static uint32_t last_version_check = 0;

// Callback for progress
typedef void (*http_ota_progress_cb_t)(int percent);
static http_ota_progress_cb_t http_ota_callback = nullptr;

// ============================================================
// Functions
// ============================================================

void http_ota_set_callback(http_ota_progress_cb_t cb) {
    http_ota_callback = cb;
}

/**
 * Compare version strings (e.g., "1.0.0" vs "1.0.1")
 * Returns: 1 if v1 > v2, -1 if v1 < v2, 0 if equal
 */
int compare_versions(const char* v1, const char* v2) {
    int major1, minor1, patch1;
    int major2, minor2, patch2;
    
    sscanf(v1, "%d.%d.%d", &major1, &minor1, &patch1);
    sscanf(v2, "%d.%d.%d", &major2, &minor2, &patch2);
    
    if (major1 != major2) return (major1 > major2) ? 1 : -1;
    if (minor1 != minor2) return (minor1 > minor2) ? 1 : -1;
    if (patch1 != patch2) return (patch1 > patch2) ? 1 : -1;
    return 0;
}

/**
 * Check if new version is available on GitHub
 */
bool http_ota_check_update() {
    if (!WiFi.isConnected()) {
        Serial.println("[HTTP-OTA] WiFi not connected");
        return false;
    }
    
    Serial.println("[HTTP-OTA] Checking for updates...");
    
    HTTPClient http;
    http.begin(VERSION_CHECK_URL);
    http.setTimeout(10000);
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP-OTA] Failed to fetch version.json, code: %d\n", httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    // Parse JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.printf("[HTTP-OTA] JSON parse error: %s\n", error.c_str());
        return false;
    }
    
    const char* remote_version = doc["version"];
    const char* remote_url = doc["url"];
    
    Serial.printf("[HTTP-OTA] Current: %s, Available: %s\n", CURRENT_VERSION, remote_version);
    
    // Compare versions
    if (compare_versions(remote_version, CURRENT_VERSION) > 0) {
        new_version = String(remote_version);
        firmware_url = String(remote_url);
        Serial.printf("[HTTP-OTA] New version available: %s\n", remote_version);
        return true;
    }
    
    Serial.println("[HTTP-OTA] Already up to date");
    return false;
}

/**
 * Download and install firmware from URL
 */
bool http_ota_perform_update() {
    if (firmware_url.length() == 0) {
        Serial.println("[HTTP-OTA] No firmware URL set");
        return false;
    }
    
    if (!WiFi.isConnected()) {
        Serial.println("[HTTP-OTA] WiFi not connected");
        return false;
    }
    
    Serial.printf("[HTTP-OTA] Downloading firmware from: %s\n", firmware_url.c_str());
    http_ota_updating = true;
    http_ota_progress = 0;
    
    HTTPClient http;
    http.begin(firmware_url);
    http.setTimeout(30000);
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP-OTA] Download failed, code: %d\n", httpCode);
        http.end();
        http_ota_updating = false;
        return false;
    }
    
    int contentLength = http.getSize();
    Serial.printf("[HTTP-OTA] Firmware size: %d bytes\n", contentLength);
    
    if (contentLength <= 0) {
        Serial.println("[HTTP-OTA] Invalid content length");
        http.end();
        http_ota_updating = false;
        return false;
    }
    
    // Start update
    if (!Update.begin(contentLength)) {
        Serial.printf("[HTTP-OTA] Not enough space: %s\n", Update.errorString());
        http.end();
        http_ota_updating = false;
        return false;
    }
    
    WiFiClient* stream = http.getStreamPtr();
    
    // Write firmware
    size_t written = 0;
    uint8_t buff[1024];
    
    while (http.connected() && written < contentLength) {
        size_t available = stream->available();
        if (available) {
            size_t readBytes = stream->readBytes(buff, min(available, sizeof(buff)));
            size_t writtenBytes = Update.write(buff, readBytes);
            
            if (writtenBytes != readBytes) {
                Serial.println("[HTTP-OTA] Write error");
                break;
            }
            
            written += writtenBytes;
            http_ota_progress = (written * 100) / contentLength;
            
            // Progress callback
            if (http_ota_callback) {
                http_ota_callback(http_ota_progress);
            }
            
            // Serial progress every 10%
            static int last_reported = 0;
            if (http_ota_progress / 10 != last_reported / 10) {
                Serial.printf("[HTTP-OTA] Progress: %d%%\n", http_ota_progress);
                last_reported = http_ota_progress;
            }
        }
        delay(1);
    }
    
    http.end();
    
    if (Update.end()) {
        if (Update.isFinished()) {
            Serial.println("[HTTP-OTA] Update complete! Rebooting...");
            http_ota_updating = false;
            delay(1000);
            ESP.restart();
            return true;
        }
    }
    
    Serial.printf("[HTTP-OTA] Update failed: %s\n", Update.errorString());
    http_ota_updating = false;
    return false;
}

/**
 * Check for updates periodically (call in loop)
 */
void http_ota_check_periodic() {
    if (!WiFi.isConnected()) return;
    if (http_ota_updating) return;
    
    uint32_t now = millis();
    if (now - last_version_check < CHECK_INTERVAL_MS) return;
    last_version_check = now;
    
    if (http_ota_check_update()) {
        // New version available - auto update
        Serial.println("[HTTP-OTA] Starting auto-update...");
        http_ota_perform_update();
    }
}

/**
 * Force check and update now
 */
void http_ota_force_update() {
    if (http_ota_check_update()) {
        http_ota_perform_update();
    }
}

/**
 * Get current firmware version
 */
const char* http_ota_get_version() {
    return CURRENT_VERSION;
}

/**
 * Check if update is in progress
 */
bool http_ota_is_updating() {
    return http_ota_updating;
}

/**
 * Get update progress (0-100)
 */
int http_ota_get_progress() {
    return http_ota_progress;
}

#endif // HTTP_OTA_H
