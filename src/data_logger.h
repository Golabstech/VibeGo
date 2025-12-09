/**
 * ============================================================
 * VibeGo - Data Logger
 * ============================================================
 * Logs test results to SPIFFS for reporting
 * ============================================================
 */

#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "ntp_time.h"
#include "webhook_logger.h"

// ============================================================
// Configuration
// ============================================================
#define LOG_FILE        "/test_logs.json"
#define MAX_LOG_ENTRIES 100
#define STATS_FILE      "/daily_stats.json"

// ============================================================
// Data Structures
// ============================================================
struct TestLogEntry {
    uint32_t id;
    String datetime;
    float bac;
    bool is_safe;
};

struct DailyStats {
    String date;
    uint16_t total_tests;
    uint16_t safe_count;
    uint16_t danger_count;
    float avg_bac;
    float max_bac;
};

// ============================================================
// State
// ============================================================
static bool logger_initialized = false;
static uint32_t next_log_id = 1;
static DailyStats today_stats = {"", 0, 0, 0, 0.0f, 0.0f};

// ============================================================
// Functions
// ============================================================

/**
 * Initialize SPIFFS and load existing data
 */
bool logger_init() {
    if (logger_initialized) return true;
    
    Serial.println("[LOGGER] Initializing SPIFFS...");
    
    if (!SPIFFS.begin(true)) {
        Serial.println("[LOGGER] SPIFFS mount failed!");
        return false;
    }
    
    // Get total and used space
    size_t total = SPIFFS.totalBytes();
    size_t used = SPIFFS.usedBytes();
    Serial.printf("[LOGGER] SPIFFS: %u/%u bytes used\n", used, total);
    
    // Load next ID from existing logs
    if (SPIFFS.exists(LOG_FILE)) {
        File file = SPIFFS.open(LOG_FILE, "r");
        if (file) {
            StaticJsonDocument<8192> doc;
            DeserializationError error = deserializeJson(doc, file);
            file.close();
            
            if (!error) {
                JsonArray logs = doc["logs"].as<JsonArray>();
                if (logs.size() > 0) {
                    next_log_id = logs[logs.size() - 1]["id"].as<uint32_t>() + 1;
                }
            }
        }
    }
    
    // Initialize today's stats
    today_stats.date = ntp_get_date();
    
    logger_initialized = true;
    Serial.printf("[LOGGER] Ready. Next ID: %lu\n", next_log_id);
    return true;
}

/**
 * Log a test result
 */
bool logger_add_test(float bac, bool is_safe) {
    if (!logger_initialized) {
        if (!logger_init()) return false;
    }
    
    // Create entry
    TestLogEntry entry;
    entry.id = next_log_id++;
    entry.datetime = ntp_get_datetime();
    entry.bac = bac;
    entry.is_safe = is_safe;
    
    Serial.printf("[LOGGER] Adding test #%lu: BAC=%.2f, Safe=%s\n", 
        entry.id, bac, is_safe ? "Yes" : "No");
    
    // Load existing logs
    StaticJsonDocument<8192> doc;
    JsonArray logs;
    
    if (SPIFFS.exists(LOG_FILE)) {
        File file = SPIFFS.open(LOG_FILE, "r");
        if (file) {
            deserializeJson(doc, file);
            file.close();
            logs = doc["logs"].as<JsonArray>();
        }
    }
    
    if (logs.isNull()) {
        logs = doc.createNestedArray("logs");
    }
    
    // Remove old entries if over limit
    while (logs.size() >= MAX_LOG_ENTRIES) {
        logs.remove(0);
    }
    
    // Add new entry
    JsonObject newEntry = logs.createNestedObject();
    newEntry["id"] = entry.id;
    newEntry["datetime"] = entry.datetime;
    newEntry["bac"] = entry.bac;
    newEntry["safe"] = entry.is_safe;
    
    // Save to file
    File file = SPIFFS.open(LOG_FILE, "w");
    if (!file) {
        Serial.println("[LOGGER] Failed to open file for writing");
        return false;
    }
    serializeJson(doc, file);
    file.close();
    
    // Update daily stats
    String today = ntp_get_date();
    if (today_stats.date != today) {
        // New day, reset stats
        today_stats.date = today;
        today_stats.total_tests = 0;
        today_stats.safe_count = 0;
        today_stats.danger_count = 0;
        today_stats.avg_bac = 0;
        today_stats.max_bac = 0;
    }
    
    today_stats.total_tests++;
    if (is_safe) {
        today_stats.safe_count++;
    } else {
        today_stats.danger_count++;
    }
    today_stats.avg_bac = ((today_stats.avg_bac * (today_stats.total_tests - 1)) + bac) / today_stats.total_tests;
    if (bac > today_stats.max_bac) {
        today_stats.max_bac = bac;
    }
    
    // Send to webhook (Make.com → Google Sheets)
    webhook_log_test(bac, is_safe, entry.datetime.c_str());
    
    return true;
}

/**
 * Get daily statistics as JSON
 */
String logger_get_stats_json() {
    StaticJsonDocument<256> doc;
    
    doc["date"] = today_stats.date;
    doc["total_tests"] = today_stats.total_tests;
    doc["safe_count"] = today_stats.safe_count;
    doc["danger_count"] = today_stats.danger_count;
    doc["avg_bac"] = String(today_stats.avg_bac, 2);
    doc["max_bac"] = String(today_stats.max_bac, 2);
    doc["safe_percent"] = today_stats.total_tests > 0 ? 
        (today_stats.safe_count * 100 / today_stats.total_tests) : 0;
    
    String output;
    serializeJson(doc, output);
    return output;
}

/**
 * Get test logs as JSON (last N entries)
 */
String logger_get_logs_json(int limit = 20) {
    if (!SPIFFS.exists(LOG_FILE)) {
        return "{\"logs\":[]}";
    }
    
    File file = SPIFFS.open(LOG_FILE, "r");
    if (!file) {
        return "{\"logs\":[]}";
    }
    
    StaticJsonDocument<8192> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        return "{\"logs\":[]}";
    }
    
    // Return last N entries
    JsonArray logs = doc["logs"].as<JsonArray>();
    
    StaticJsonDocument<4096> result;
    JsonArray resultLogs = result.createNestedArray("logs");
    
    int start = logs.size() > limit ? logs.size() - limit : 0;
    for (int i = logs.size() - 1; i >= start; i--) {
        resultLogs.add(logs[i]);
    }
    
    String output;
    serializeJson(result, output);
    return output;
}

/**
 * Get daily statistics
 */
DailyStats logger_get_stats() {
    return today_stats;
}

/**
 * Get total test count (all time)
 */
uint32_t logger_get_total_tests() {
    return next_log_id - 1;
}

/**
 * Clear all logs
 */
void logger_clear() {
    if (SPIFFS.exists(LOG_FILE)) {
        SPIFFS.remove(LOG_FILE);
    }
    next_log_id = 1;
    today_stats = {"", 0, 0, 0, 0.0f, 0.0f};
    Serial.println("[LOGGER] Logs cleared");
}

#endif // DATA_LOGGER_H
