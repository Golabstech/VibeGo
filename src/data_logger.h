/**
 * ============================================================
 * VibeGo - Data Logger v2.0
 * ============================================================
 * Logs test results to SPIFFS for reporting
 * Supports date range filtering and CSV export
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
#define MAX_LOG_ENTRIES 500  // Increased for historical data
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

// Filter period enum
typedef enum {
    FILTER_TODAY = 0,
    FILTER_WEEK = 1,
    FILTER_MONTH = 2,
    FILTER_SIX_MONTHS = 3,
    FILTER_ALL = 4
} filter_period_t;

// ============================================================
// State
// ============================================================
static bool logger_initialized = false;
static uint32_t next_log_id = 1;
static DailyStats today_stats = {"", 0, 0, 0, 0.0f, 0.0f};

// ============================================================
// Helper Functions
// ============================================================

/**
 * Parse date string (YYYY-MM-DD) to components
 */
bool parse_date(const String& dateStr, int& year, int& month, int& day) {
    if (dateStr.length() < 10) return false;
    year = dateStr.substring(0, 4).toInt();
    month = dateStr.substring(5, 7).toInt();
    day = dateStr.substring(8, 10).toInt();
    return year > 2000 && month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

/**
 * Calculate days difference between two dates
 */
int days_ago(const String& dateStr) {
    int year, month, day;
    if (!parse_date(dateStr, year, month, day)) return 9999;
    
    int tyear, tmonth, tday;
    String today = ntp_get_date();
    if (!parse_date(today, tyear, tmonth, tday)) return 0;
    
    // Simple approximation (not perfect but good enough for filtering)
    int todayDays = tyear * 365 + tmonth * 30 + tday;
    int dateDays = year * 365 + month * 30 + day;
    
    return todayDays - dateDays;
}

/**
 * Check if date is within filter period
 */
bool is_within_period(const String& datetime, filter_period_t period) {
    if (period == FILTER_ALL) return true;
    
    String dateStr = datetime.substring(0, 10);
    int daysAgo = days_ago(dateStr);
    
    switch (period) {
        case FILTER_TODAY:      return daysAgo == 0;
        case FILTER_WEEK:       return daysAgo <= 7;
        case FILTER_MONTH:      return daysAgo <= 30;
        case FILTER_SIX_MONTHS: return daysAgo <= 180;
        default:                return true;
    }
}

// ============================================================
// Main Functions
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
            DynamicJsonDocument doc(16384);
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
    DynamicJsonDocument doc(16384);
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
 * Get statistics for a given period as JSON
 */
String logger_get_stats_json_filtered(filter_period_t period = FILTER_TODAY) {
    if (!SPIFFS.exists(LOG_FILE)) {
        return "{\"date\":\"\",\"total_tests\":0,\"safe_count\":0,\"danger_count\":0,\"avg_bac\":\"0.00\",\"max_bac\":\"0.00\",\"safe_percent\":0}";
    }
    
    File file = SPIFFS.open(LOG_FILE, "r");
    if (!file) {
        return "{\"date\":\"\",\"total_tests\":0,\"safe_count\":0,\"danger_count\":0,\"avg_bac\":\"0.00\",\"max_bac\":\"0.00\",\"safe_percent\":0}";
    }
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        return "{\"date\":\"\",\"total_tests\":0,\"safe_count\":0,\"danger_count\":0,\"avg_bac\":\"0.00\",\"max_bac\":\"0.00\",\"safe_percent\":0}";
    }
    
    JsonArray logs = doc["logs"].as<JsonArray>();
    
    // Calculate stats for period
    int total = 0, safe = 0, danger = 0;
    float sumBac = 0, maxBac = 0;
    
    for (JsonVariant v : logs) {
        String datetime = v["datetime"].as<String>();
        if (is_within_period(datetime, period)) {
            total++;
            float bac = v["bac"].as<float>();
            bool isSafe = v["safe"].as<bool>();
            
            if (isSafe) safe++;
            else danger++;
            
            sumBac += bac;
            if (bac > maxBac) maxBac = bac;
        }
    }
    
    StaticJsonDocument<256> result;
    result["total_tests"] = total;
    result["safe_count"] = safe;
    result["danger_count"] = danger;
    result["avg_bac"] = total > 0 ? String(sumBac / total, 2) : "0.00";
    result["max_bac"] = String(maxBac, 2);
    result["safe_percent"] = total > 0 ? (safe * 100 / total) : 0;
    
    String output;
    serializeJson(result, output);
    return output;
}

/**
 * Get daily statistics as JSON (backward compatible)
 */
String logger_get_stats_json() {
    return logger_get_stats_json_filtered(FILTER_TODAY);
}

/**
 * Get test logs as JSON with period filter
 */
String logger_get_logs_json_filtered(filter_period_t period = FILTER_ALL, int limit = 100) {
    if (!SPIFFS.exists(LOG_FILE)) {
        return "{\"logs\":[]}";
    }
    
    File file = SPIFFS.open(LOG_FILE, "r");
    if (!file) {
        return "{\"logs\":[]}";
    }
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        return "{\"logs\":[]}";
    }
    
    JsonArray logs = doc["logs"].as<JsonArray>();
    
    DynamicJsonDocument result(16384);
    JsonArray resultLogs = result.createNestedArray("logs");
    
    int count = 0;
    // Iterate from newest to oldest
    for (int i = logs.size() - 1; i >= 0 && count < limit; i--) {
        String datetime = logs[i]["datetime"].as<String>();
        if (is_within_period(datetime, period)) {
            resultLogs.add(logs[i]);
            count++;
        }
    }
    
    String output;
    serializeJson(result, output);
    return output;
}

/**
 * Get test logs as JSON (backward compatible)
 */
String logger_get_logs_json(int limit = 20) {
    return logger_get_logs_json_filtered(FILTER_ALL, limit);
}

/**
 * Export logs as CSV with period filter
 */
String logger_export_csv(filter_period_t period = FILTER_ALL) {
    String csv = "ID,Tarih/Saat,BAC,Sonuc\n";
    
    if (!SPIFFS.exists(LOG_FILE)) {
        return csv;
    }
    
    File file = SPIFFS.open(LOG_FILE, "r");
    if (!file) {
        return csv;
    }
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        return csv;
    }
    
    JsonArray logs = doc["logs"].as<JsonArray>();
    
    for (JsonVariant v : logs) {
        String datetime = v["datetime"].as<String>();
        if (is_within_period(datetime, period)) {
            csv += String(v["id"].as<uint32_t>()) + ",";
            csv += datetime + ",";
            csv += String(v["bac"].as<float>(), 2) + ",";
            csv += v["safe"].as<bool>() ? "Guvenli" : "Tehlikeli";
            csv += "\n";
        }
    }
    
    return csv;
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
