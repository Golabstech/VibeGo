/**
 * ============================================================
 * VibeGo - Sponsor Manager
 * ============================================================
 * Manages sponsor settings, QR URL, and scan analytics
 * Settings are stored in NVS (Preferences)
 * ============================================================
 */

#ifndef SPONSOR_MANAGER_H
#define SPONSOR_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>

// ============================================================
// Configuration
// ============================================================
#define DEFAULT_QR_URL      "https://vibego.app/go"
#define DEFAULT_SPONSOR_NAME "VibeGo"
#define DEFAULT_QR_TITLE    "TAKSI CAG"

// ============================================================
// State
// ============================================================
static Preferences sponsor_prefs;
static String sponsor_qr_url = DEFAULT_QR_URL;
static String sponsor_name = DEFAULT_SPONSOR_NAME;
static String sponsor_phone = "";
static String sponsor_qr_title = DEFAULT_QR_TITLE;
static uint32_t qr_scan_count = 0;
static bool sponsor_initialized = false;

// ============================================================
// Functions
// ============================================================

/**
 * Initialize sponsor manager and load saved settings
 */
void sponsor_init() {
    if (sponsor_initialized) return;
    
    sponsor_prefs.begin("sponsor", true);  // Read-only first
    sponsor_qr_url = sponsor_prefs.getString("qr_url", DEFAULT_QR_URL);
    sponsor_name = sponsor_prefs.getString("name", DEFAULT_SPONSOR_NAME);
    sponsor_phone = sponsor_prefs.getString("phone", "");
    sponsor_qr_title = sponsor_prefs.getString("qr_title", DEFAULT_QR_TITLE);
    qr_scan_count = sponsor_prefs.getUInt("scans", 0);
    sponsor_prefs.end();
    
    sponsor_initialized = true;
    Serial.printf("[SPONSOR] Loaded: %s (%s)\n", sponsor_name.c_str(), sponsor_qr_url.c_str());
}

/**
 * Save sponsor settings
 */
void sponsor_save() {
    sponsor_prefs.begin("sponsor", false);  // Read-write
    sponsor_prefs.putString("qr_url", sponsor_qr_url);
    sponsor_prefs.putString("name", sponsor_name);
    sponsor_prefs.putString("phone", sponsor_phone);
    sponsor_prefs.putString("qr_title", sponsor_qr_title);
    sponsor_prefs.putUInt("scans", qr_scan_count);
    sponsor_prefs.end();
    
    Serial.println("[SPONSOR] Settings saved");
}

/**
 * Set QR URL
 */
void sponsor_set_qr_url(const String& url) {
    sponsor_qr_url = url;
    sponsor_save();
}

/**
 * Set sponsor name
 */
void sponsor_set_name(const String& name) {
    sponsor_name = name;
    sponsor_save();
}

/**
 * Set sponsor phone
 */
void sponsor_set_phone(const String& phone) {
    sponsor_phone = phone;
    sponsor_save();
}

/**
 * Set QR title
 */
void sponsor_set_qr_title(const String& title) {
    sponsor_qr_title = title;
    sponsor_save();
}

/**
 * Increment QR scan count (called when redirect page is accessed)
 */
void sponsor_increment_scan() {
    qr_scan_count++;
    // Save periodically (every 10 scans) to reduce flash writes
    if (qr_scan_count % 10 == 0) {
        sponsor_save();
    }
}

/**
 * Reset scan count
 */
void sponsor_reset_scans() {
    qr_scan_count = 0;
    sponsor_save();
}

// ============================================================
// Getters
// ============================================================
String sponsor_get_qr_url() {
    if (!sponsor_initialized) sponsor_init();
    return sponsor_qr_url;
}

String sponsor_get_name() {
    if (!sponsor_initialized) sponsor_init();
    return sponsor_name;
}

String sponsor_get_phone() {
    if (!sponsor_initialized) sponsor_init();
    return sponsor_phone;
}

String sponsor_get_qr_title() {
    if (!sponsor_initialized) sponsor_init();
    return sponsor_qr_title;
}

uint32_t sponsor_get_scan_count() {
    if (!sponsor_initialized) sponsor_init();
    return qr_scan_count;
}

/**
 * Get sponsor info as JSON
 */
String sponsor_get_json() {
    if (!sponsor_initialized) sponsor_init();
    
    StaticJsonDocument<320> doc;
    doc["qr_url"] = sponsor_qr_url;
    doc["name"] = sponsor_name;
    doc["phone"] = sponsor_phone;
    doc["qr_title"] = sponsor_qr_title;
    doc["scan_count"] = qr_scan_count;
    
    String output;
    serializeJson(doc, output);
    return output;
}

#endif // SPONSOR_MANAGER_H
