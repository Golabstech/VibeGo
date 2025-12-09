/**
 * ============================================================
 * VibeGo - Webhook Logger with Sync Interval
 * ============================================================
 * Sends test data to webhook with configurable sync interval
 * ============================================================
 */

#ifndef WEBHOOK_LOGGER_H
#define WEBHOOK_LOGGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

// ============================================================
// Configuration
// ============================================================
#define WEBHOOK_URL "https://hook.eu1.make.com/qz8uo9lfl9lvt6mhrrkijh2yeggmuht5"
#define WEBHOOK_ENABLED true
#define WEBHOOK_MAX_QUEUE 50

// ============================================================
// Sync Interval Options (in minutes)
// 0 = Immediate, 1 = 1min, 10 = 10min, 30 = 30min
// ============================================================
enum WebhookSyncInterval {
    SYNC_IMMEDIATE = 0,
    SYNC_1MIN = 1,
    SYNC_10MIN = 10,
    SYNC_30MIN = 30,
    SYNC_60MIN = 60,
    SYNC_1DAY = 1440,
};

// ============================================================
// Test Data Structure
// ============================================================
struct WebhookEntry {
    String datetime;
    float bac;
    bool is_safe;
};

// ============================================================
// State
// ============================================================
static bool webhook_available = false;
static WebhookSyncInterval sync_interval = SYNC_1DAY;  // Varsayılan: günlük
static std::vector<WebhookEntry> webhook_queue;
static unsigned long last_sync_time = 0;
static int pending_count = 0;

// ============================================================
// Functions
// ============================================================

void webhook_init() {
    if (!WEBHOOK_ENABLED || strlen(WEBHOOK_URL) < 10) {
        Serial.println("[WEBHOOK] Disabled");
        return;
    }
    webhook_available = true;
    Serial.println("[WEBHOOK] Ready (Make.com)");
}

// Set sync interval (0=immediate, 1=1min, 10=10min, 30=30min, 60=1hr, 1440=1day)
void webhook_set_interval(int minutes) {
    if (minutes <= 0) sync_interval = SYNC_IMMEDIATE;
    else if (minutes <= 1) sync_interval = SYNC_1MIN;
    else if (minutes <= 10) sync_interval = SYNC_10MIN;
    else if (minutes <= 30) sync_interval = SYNC_30MIN;
    else if (minutes <= 60) sync_interval = SYNC_60MIN;
    else sync_interval = SYNC_1DAY;
    Serial.printf("[WEBHOOK] Sync interval: %d min\n", sync_interval);
}

int webhook_get_interval() {
    return (int)sync_interval;
}

int webhook_get_pending_count() {
    return webhook_queue.size();
}

// Send single entry to webhook
bool webhook_send_entry(const WebhookEntry& entry) {
    if (!WiFi.isConnected()) return false;
    
    HTTPClient http;
    http.begin(WEBHOOK_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.setTimeout(10000);
    
    String payload = "datetime=" + entry.datetime;
    payload += "&bac=" + String(entry.bac, 2);
    payload += "&result=" + String(entry.is_safe ? "Guvenli" : "Tehlikeli");
    payload += "&device=VibeGo";
    
    int httpCode = http.POST(payload);
    http.end();
    
    return (httpCode >= 200 && httpCode < 300);
}

// Flush queue - send all pending entries
int webhook_flush_queue() {
    if (webhook_queue.empty() || !webhook_available || !WiFi.isConnected()) {
        return 0;
    }
    
    Serial.printf("[WEBHOOK] Syncing %d entries...\n", webhook_queue.size());
    
    int success_count = 0;
    for (const auto& entry : webhook_queue) {
        if (webhook_send_entry(entry)) {
            success_count++;
        }
        delay(100); // Small delay between requests
    }
    
    Serial.printf("[WEBHOOK] Synced %d/%d\n", success_count, webhook_queue.size());
    webhook_queue.clear();
    last_sync_time = millis();
    
    return success_count;
}

// Add test to queue or send immediately
void webhook_log_test(float bac, bool is_safe, const char* datetime) {
    if (!webhook_available) return;
    
    WebhookEntry entry;
    entry.datetime = String(datetime);
    entry.bac = bac;
    entry.is_safe = is_safe;
    
    if (sync_interval == SYNC_IMMEDIATE) {
        // Send immediately
        Serial.println("[WEBHOOK] Sending...");
        if (webhook_send_entry(entry)) {
            Serial.println("[WEBHOOK] OK");
        } else {
            // Failed, add to queue for retry
            if (webhook_queue.size() < WEBHOOK_MAX_QUEUE) {
                webhook_queue.push_back(entry);
            }
        }
    } else {
        // Add to queue
        if (webhook_queue.size() < WEBHOOK_MAX_QUEUE) {
            webhook_queue.push_back(entry);
            Serial.printf("[WEBHOOK] Queued (%d pending)\n", webhook_queue.size());
        }
    }
}

// Check if it's time to sync (call from loop)
void webhook_update() {
    if (!webhook_available || sync_interval == SYNC_IMMEDIATE) return;
    if (webhook_queue.empty()) return;
    
    unsigned long interval_ms = sync_interval * 60 * 1000;
    if (millis() - last_sync_time >= interval_ms) {
        webhook_flush_queue();
    }
}

// Force sync now
int webhook_sync_now() {
    return webhook_flush_queue();
}

bool webhook_is_available() {
    return webhook_available;
}

#endif // WEBHOOK_LOGGER_H
