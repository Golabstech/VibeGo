/**
 * ============================================================
 * VibeGo - NTP Time Synchronization (Non-blocking)
 * ============================================================
 * Gets accurate time from NTP servers for logging
 * Non-blocking implementation to avoid watchdog triggers
 * ============================================================
 */

#ifndef NTP_TIME_H
#define NTP_TIME_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <esp_task_wdt.h>

// ============================================================
// Configuration
// ============================================================
// Primary: Turkey-specific, Secondary: Global reliable servers
#define NTP_SERVER1     "tr.pool.ntp.org"    // Turkey NTP pool
#define NTP_SERVER2     "time.google.com"    // Google (fast, reliable)
#define NTP_SERVER3     "pool.ntp.org"       // Global fallback
#define NTP_GMT_OFFSET  10800                // Turkey GMT+3 = 3*3600
#define NTP_DST_OFFSET  0                    // No DST in Turkey
#define NTP_SYNC_TIMEOUT_MS  15000           // 15 seconds timeout (was 5s)
#define NTP_RETRY_INTERVAL   60000           // Retry every 60 seconds if failed
#define NTP_MAX_RETRIES      5               // Max retry attempts

// ============================================================
// State
// ============================================================
static bool ntp_synced = false;
static bool ntp_init_done = false;
static unsigned long ntp_sync_start = 0;
static bool ntp_syncing = false;
static int ntp_retry_count = 0;
static unsigned long ntp_last_retry = 0;
static bool ntp_wifi_was_connected = false;

// ============================================================
// Functions
// ============================================================

/**
 * Start NTP time sync (non-blocking)
 * Call after WiFi is connected
 */
void ntp_init() {
    if (!WiFi.isConnected()) {
        Serial.println("[NTP] WiFi not connected, skipping");
        return;
    }
    
    Serial.println("[NTP] Starting time sync...");
    Serial.printf("[NTP] Servers: %s, %s, %s\n", NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    
    configTime(NTP_GMT_OFFSET, NTP_DST_OFFSET, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    
    ntp_init_done = true;
    ntp_syncing = true;
    ntp_sync_start = millis();
    ntp_wifi_was_connected = true;
}

/**
 * Update NTP sync status (call in loop)
 * Returns true when sync is complete (success or timeout)
 * Also handles retries and WiFi reconnection
 */
bool ntp_update() {
    // Check if WiFi was disconnected and reconnected
    bool wifi_connected = WiFi.isConnected();
    if (wifi_connected && !ntp_wifi_was_connected && !ntp_synced) {
        // WiFi just reconnected, restart NTP sync
        Serial.println("[NTP] WiFi reconnected, restarting sync...");
        ntp_init();
    }
    ntp_wifi_was_connected = wifi_connected;
    
    // If not syncing, check if we should retry
    if (!ntp_syncing) {
        // Retry if not synced and haven't exceeded max retries
        if (!ntp_synced && ntp_retry_count < NTP_MAX_RETRIES && wifi_connected) {
            if (millis() - ntp_last_retry > NTP_RETRY_INTERVAL) {
                ntp_retry_count++;
                ntp_last_retry = millis();
                Serial.printf("[NTP] Retry %d/%d...\n", ntp_retry_count, NTP_MAX_RETRIES);
                ntp_init();
            }
        }
        return true;
    }
    
    // Check if synced
    struct tm timeinfo;
    if (getLocalTime(&timeinfo, 0)) {  // 0 = don't block
        ntp_synced = true;
        ntp_syncing = false;
        ntp_retry_count = 0;  // Reset retry count on success
        Serial.printf("[NTP] ✓ Time synced: %04d-%02d-%02d %02d:%02d:%02d\n",
            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        return true;
    }
    
    // Check timeout
    if (millis() - ntp_sync_start > NTP_SYNC_TIMEOUT_MS) {
        ntp_syncing = false;
        ntp_last_retry = millis();
        Serial.printf("[NTP] Sync timeout (retry %d/%d)\n", ntp_retry_count, NTP_MAX_RETRIES);
        return true;
    }
    
    return false;
}

/**
 * Check if time is synced
 */
bool ntp_is_synced() {
    return ntp_synced;
}

/**
 * Check if sync is in progress
 */
bool ntp_is_syncing() {
    return ntp_syncing;
}

/**
 * Get current timestamp (seconds since epoch)
 */
time_t ntp_get_timestamp() {
    time_t now;
    time(&now);
    return now;
}

/**
 * Get formatted date string (YYYY-MM-DD)
 */
String ntp_get_date() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 0)) {
        return "----.--.--";
    }
    char buf[12];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", 
        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
    return String(buf);
}

/**
 * Get formatted time string (HH:MM:SS)
 */
String ntp_get_time() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 0)) {
        return "--:--:--";
    }
    char buf[10];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 
        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    return String(buf);
}

/**
 * Get formatted datetime string (YYYY-MM-DD HH:MM:SS)
 */
String ntp_get_datetime() {
    return ntp_get_date() + " " + ntp_get_time();
}

/**
 * Get uptime in seconds
 */
uint32_t ntp_get_uptime() {
    return millis() / 1000;
}

/**
 * Get formatted uptime string (Xd Xh Xm)
 */
String ntp_get_uptime_str() {
    uint32_t uptime = ntp_get_uptime();
    uint32_t days = uptime / 86400;
    uint32_t hours = (uptime % 86400) / 3600;
    uint32_t mins = (uptime % 3600) / 60;
    
    char buf[20];
    snprintf(buf, sizeof(buf), "%lud %luh %lum", days, hours, mins);
    return String(buf);
}

#endif // NTP_TIME_H
