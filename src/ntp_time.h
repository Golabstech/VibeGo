/**
 * ============================================================
 * VibeGo - NTP Time Synchronization
 * ============================================================
 * Gets accurate time from NTP servers for logging
 * ============================================================
 */

#ifndef NTP_TIME_H
#define NTP_TIME_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// ============================================================
// Configuration
// ============================================================
#define NTP_SERVER1     "pool.ntp.org"
#define NTP_SERVER2     "time.google.com"
#define NTP_GMT_OFFSET  10800      // Turkey GMT+3 = 3*3600
#define NTP_DST_OFFSET  0          // No DST in Turkey

// ============================================================
// State
// ============================================================
static bool ntp_synced = false;

// ============================================================
// Functions
// ============================================================

/**
 * Initialize NTP time sync
 * Call after WiFi is connected
 */
void ntp_init() {
    if (!WiFi.isConnected()) {
        Serial.println("[NTP] WiFi not connected");
        return;
    }
    
    Serial.println("[NTP] Syncing time...");
    configTime(NTP_GMT_OFFSET, NTP_DST_OFFSET, NTP_SERVER1, NTP_SERVER2);
    
    // Wait for sync (max 10 seconds)
    struct tm timeinfo;
    int retry = 0;
    while (!getLocalTime(&timeinfo) && retry < 10) {
        delay(1000);
        retry++;
    }
    
    if (retry < 10) {
        ntp_synced = true;
        Serial.printf("[NTP] Time synced: %04d-%02d-%02d %02d:%02d:%02d\n",
            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    } else {
        Serial.println("[NTP] Sync failed");
    }
}

/**
 * Check if time is synced
 */
bool ntp_is_synced() {
    return ntp_synced;
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
    if (!getLocalTime(&timeinfo)) {
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
    if (!getLocalTime(&timeinfo)) {
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
