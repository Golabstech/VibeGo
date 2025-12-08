/**
 * ============================================================
 * VibeGo - WiFi Manager Module
 * ============================================================
 * Handles WiFi connection using WiFiManager captive portal
 * ============================================================
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

// ============================================================
// Configuration
// ============================================================
#define WIFI_AP_NAME      "VibeGo-Setup"
#define WIFI_AP_PASSWORD  ""              // Open AP for easy setup
#define WIFI_TIMEOUT_SEC  180             // 3 minutes portal timeout

// ============================================================
// WiFi Status
// ============================================================
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_AP_MODE
} wifi_status_t;

// ============================================================
// Global Variables
// ============================================================
static WiFiManager wm;
static wifi_status_t current_wifi_status = WIFI_STATUS_DISCONNECTED;
static bool wifi_initialized = false;

// ============================================================
// Functions
// ============================================================

/**
 * Initialize WiFi with auto-connect or captive portal
 * @param blocking If true, blocks until connected or timeout
 * @return true if connected to WiFi
 */
bool wifi_init(bool blocking = false) {
    if (wifi_initialized) return WiFi.isConnected();
    
    Serial.println("[WIFI] Initializing...");
    
    // Configure WiFiManager
    wm.setConfigPortalTimeout(WIFI_TIMEOUT_SEC);
    wm.setConnectTimeout(20);  // Reduced timeout
    
    // Set dark theme for captive portal
    wm.setClass("invert");
    
    // Custom menu items
    std::vector<const char*> menu = {"wifi", "info", "sep", "restart"};
    wm.setMenu(menu);
    
    // WiFi mode
    WiFi.mode(WIFI_STA);
    
    wifi_initialized = true;
    
    if (blocking) {
        // Blocking mode - start portal and wait
        current_wifi_status = WIFI_STATUS_AP_MODE;
        Serial.println("[WIFI] Starting config portal (blocking)...");
        bool connected = wm.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD);
        if (connected) {
            current_wifi_status = WIFI_STATUS_CONNECTED;
            Serial.print("[WIFI] Connected! IP: ");
            Serial.println(WiFi.localIP());
            return true;
        }
    } else {
        // Non-blocking - just try to connect with saved credentials
        current_wifi_status = WIFI_STATUS_CONNECTING;
        
        // Try to connect with saved credentials
        WiFi.begin();
        Serial.println("[WIFI] Trying saved credentials...");
        
        // Wait briefly for connection
        unsigned long start = millis();
        while (millis() - start < 5000) {
            if (WiFi.isConnected()) {
                current_wifi_status = WIFI_STATUS_CONNECTED;
                Serial.print("[WIFI] Connected! IP: ");
                Serial.println(WiFi.localIP());
                return true;
            }
            delay(100);
        }
        
        Serial.println("[WIFI] Not connected. Use 'wifi_portal' to configure.");
        current_wifi_status = WIFI_STATUS_DISCONNECTED;
    }
    
    return false;
}

/**
 * Start configuration portal manually
 */
void wifi_start_portal() {
    Serial.println("[WIFI] Starting config portal...");
    current_wifi_status = WIFI_STATUS_AP_MODE;
    wm.startConfigPortal(WIFI_AP_NAME, WIFI_AP_PASSWORD);
}

/**
 * Update WiFi status (call in loop for non-blocking mode)
 */
void wifi_update() {
    if (!wifi_initialized) return;
    
    if (WiFi.isConnected()) {
        if (current_wifi_status != WIFI_STATUS_CONNECTED) {
            current_wifi_status = WIFI_STATUS_CONNECTED;
            Serial.print("[WIFI] Connected! IP: ");
            Serial.println(WiFi.localIP());
        }
    } else {
        if (current_wifi_status == WIFI_STATUS_CONNECTED) {
            current_wifi_status = WIFI_STATUS_DISCONNECTED;
            Serial.println("[WIFI] Disconnected!");
        }
    }
}

/**
 * Get current WiFi status
 */
wifi_status_t wifi_get_status() {
    return current_wifi_status;
}

/**
 * Get WiFi status string
 */
const char* wifi_get_status_str() {
    switch (current_wifi_status) {
        case WIFI_STATUS_CONNECTED:    return "Bagli";
        case WIFI_STATUS_CONNECTING:   return "Baglaniyor...";
        case WIFI_STATUS_AP_MODE:      return "AP Modu";
        default:                       return "Bagli Degil";
    }
}

/**
 * Get IP address string
 */
String wifi_get_ip() {
    if (WiFi.isConnected()) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}

/**
 * Get SSID
 */
String wifi_get_ssid() {
    if (WiFi.isConnected()) {
        return WiFi.SSID();
    }
    return "";
}

/**
 * Get signal strength (0-100%)
 */
int wifi_get_signal_percent() {
    if (!WiFi.isConnected()) return 0;
    
    int rssi = WiFi.RSSI();
    // Convert RSSI to percentage (-100dBm = 0%, -50dBm = 100%)
    int percent = 2 * (rssi + 100);
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    return percent;
}

/**
 * Reset WiFi settings
 */
void wifi_reset() {
    Serial.println("[WIFI] Resetting settings...");
    wm.resetSettings();
    ESP.restart();
}

#endif // WIFI_MANAGER_H
