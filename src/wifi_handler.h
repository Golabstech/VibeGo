/**
 * ============================================================
 * VibeGo - WiFi Module (AP + STA Mode with Timeout)
 * ============================================================
 * ESP32 runs in AP+STA mode:
 * - AP mode: Active for first 30 minutes for configuration
 * - After timeout: AP turns off if connected to external network
 * - STA mode: Connect to external WiFi for internet
 * ============================================================
 */

#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

// ============================================================
// Configuration
// ============================================================
#define WIFI_AP_SSID        "VibeGo"
#define WIFI_AP_PASSWORD    "12345678"      // Min 8 chars for WPA2
#define WIFI_AP_CHANNEL     1
#define WIFI_CONNECT_TIMEOUT 15000          // 15 seconds connection timeout
#define WIFI_AP_TIMEOUT_MS   (10 * 60 * 1000)  // 30 minutes AP active time

// ============================================================
// WiFi Status
// ============================================================
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_AP_ONLY
} wifi_status_t;

// ============================================================
// WiFi Network Info
// ============================================================
typedef struct {
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    bool isOpen;
} wifi_network_t;

// ============================================================
// Global Variables
// ============================================================
static Preferences wifi_prefs;
static wifi_status_t current_wifi_status = WIFI_STATUS_DISCONNECTED;
static bool wifi_initialized = false;
static String saved_ssid = "";
static String saved_password = "";
static IPAddress ap_ip(192, 168, 4, 1);
static IPAddress ap_gateway(192, 168, 4, 1);
static IPAddress ap_subnet(255, 255, 255, 0);

static wifi_network_t scanned_networks[20];
static int scanned_network_count = 0;

// AP timeout management
static unsigned long ap_start_time = 0;
static bool ap_active = false;
static bool ap_timeout_occurred = false;

// ============================================================
// Load saved WiFi credentials
// ============================================================
bool wifi_load_credentials() {
    wifi_prefs.begin("wifi", true);  // Read-only
    saved_ssid = wifi_prefs.getString("ssid", "");
    saved_password = wifi_prefs.getString("password", "");
    wifi_prefs.end();
    
    if (saved_ssid.length() > 0) {
        Serial.printf("[WIFI] Loaded credentials for: %s\n", saved_ssid.c_str());
        return true;
    }
    return false;
}

// ============================================================
// Save WiFi credentials
// ============================================================
void wifi_save_credentials(const String& ssid, const String& password) {
    wifi_prefs.begin("wifi", false);  // Read-write
    wifi_prefs.putString("ssid", ssid);
    wifi_prefs.putString("password", password);
    wifi_prefs.end();
    
    saved_ssid = ssid;
    saved_password = password;
    Serial.printf("[WIFI] Saved credentials for: %s\n", ssid.c_str());
}

// ============================================================
// Clear WiFi credentials
// ============================================================
void wifi_clear_credentials() {
    wifi_prefs.begin("wifi", false);
    wifi_prefs.clear();
    wifi_prefs.end();
    
    saved_ssid = "";
    saved_password = "";
    Serial.println("[WIFI] Credentials cleared");
}

// ============================================================
// Start AP mode
// ============================================================
void wifi_start_ap() {
    if (ap_active) return;
    
    WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL);
    
    ap_active = true;
    ap_start_time = millis();
    ap_timeout_occurred = false;
    
    Serial.printf("[WIFI] AP Started: %s (Password: %s)\n", WIFI_AP_SSID, WIFI_AP_PASSWORD);
    Serial.printf("[WIFI] AP IP: %s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("[WIFI] AP will timeout in %d minutes\n", WIFI_AP_TIMEOUT_MS / 60000);
}

// ============================================================
// Stop AP mode
// ============================================================
void wifi_stop_ap() {
    if (!ap_active) return;
    
    WiFi.softAPdisconnect(true);
    ap_active = false;
    ap_timeout_occurred = true;
    
    Serial.println("[WIFI] AP Stopped (timeout reached, connected to network)");
}

// ============================================================
// Restart AP mode (manual trigger from web or serial)
// ============================================================
void wifi_restart_ap() {
    if (ap_active) {
        // Just reset the timer
        ap_start_time = millis();
        ap_timeout_occurred = false;
        Serial.println("[WIFI] AP timer reset (30 minutes)");
    } else {
        // Start AP again
        WiFi.mode(WIFI_AP_STA);
        wifi_start_ap();
    }
}

// ============================================================
// Check if AP is active
// ============================================================
bool wifi_is_ap_active() {
    return ap_active;
}

// ============================================================
// Get remaining AP time in seconds
// ============================================================
int wifi_get_ap_remaining_seconds() {
    if (!ap_active) return 0;
    
    unsigned long elapsed = millis() - ap_start_time;
    if (elapsed >= WIFI_AP_TIMEOUT_MS) return 0;
    
    return (WIFI_AP_TIMEOUT_MS - elapsed) / 1000;
}

// ============================================================
// Scan for available networks
// ============================================================
int wifi_scan_networks() {
    Serial.println("[WIFI] Scanning networks...");
    
    WiFi.scanDelete();
    int n = WiFi.scanNetworks(false, false, false, 300);
    
    scanned_network_count = min(n, 20);
    
    for (int i = 0; i < scanned_network_count; i++) {
        scanned_networks[i].ssid = WiFi.SSID(i);
        scanned_networks[i].rssi = WiFi.RSSI(i);
        scanned_networks[i].encryptionType = WiFi.encryptionType(i);
        scanned_networks[i].isOpen = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);
    }
    
    Serial.printf("[WIFI] Found %d networks\n", scanned_network_count);
    return scanned_network_count;
}

// ============================================================
// Get scanned networks as JSON
// ============================================================
String wifi_get_networks_json() {
    String json = "{\"networks\":[";
    
    for (int i = 0; i < scanned_network_count; i++) {
        if (i > 0) json += ",";
        json += "{\"ssid\":\"" + scanned_networks[i].ssid + "\",";
        json += "\"rssi\":" + String(scanned_networks[i].rssi) + ",";
        json += "\"open\":" + String(scanned_networks[i].isOpen ? "true" : "false") + "}";
    }
    
    json += "]}";
    return json;
}

// ============================================================
// Connect to WiFi network
// ============================================================
bool wifi_connect(const String& ssid, const String& password, bool save = true) {
    Serial.printf("[WIFI] Connecting to: %s\n", ssid.c_str());
    current_wifi_status = WIFI_STATUS_CONNECTING;
    
    WiFi.begin(ssid.c_str(), password.c_str());
    
    unsigned long start = millis();
    while (millis() - start < WIFI_CONNECT_TIMEOUT) {
        if (WiFi.status() == WL_CONNECTED) {
            current_wifi_status = WIFI_STATUS_CONNECTED;
            Serial.printf("[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            
            if (save) {
                wifi_save_credentials(ssid, password);
            }
            return true;
        }
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n[WIFI] Connection failed!");
    current_wifi_status = ap_active ? WIFI_STATUS_AP_ONLY : WIFI_STATUS_DISCONNECTED;
    WiFi.disconnect();
    return false;
}

// ============================================================
// Initialize WiFi in AP+STA mode
// ============================================================
bool wifi_init(bool autoConnect = true) {
    if (wifi_initialized) return WiFi.isConnected();
    
    Serial.println("[WIFI] Initializing...");
    
    // Set AP+STA mode
    WiFi.mode(WIFI_AP_STA);
    
    // Start AP
    wifi_start_ap();
    
    wifi_initialized = true;
    current_wifi_status = WIFI_STATUS_AP_ONLY;
    
    // Try to connect to saved network
    if (autoConnect && wifi_load_credentials()) {
        if (saved_ssid.length() > 0) {
            Serial.println("[WIFI] Trying saved network...");
            if (wifi_connect(saved_ssid, saved_password, false)) {
                return true;
            }
        }
    }
    
    Serial.println("[WIFI] Running in AP mode for configuration");
    Serial.printf("[WIFI] Connect to '%s' (password: %s) and open http://%s/\n", 
                  WIFI_AP_SSID, WIFI_AP_PASSWORD, WiFi.softAPIP().toString().c_str());
    
    return false;
}

// ============================================================
// Disconnect from current network
// ============================================================
void wifi_disconnect() {
    WiFi.disconnect();
    current_wifi_status = ap_active ? WIFI_STATUS_AP_ONLY : WIFI_STATUS_DISCONNECTED;
    Serial.println("[WIFI] Disconnected from network");
}

// ============================================================
// Reset WiFi (clear credentials and restart)
// ============================================================
void wifi_reset() {
    Serial.println("[WIFI] Resetting...");
    wifi_clear_credentials();
    WiFi.disconnect();
    delay(500);
    ESP.restart();
}

// ============================================================
// Update WiFi status (call in loop) - handles AP timeout
// ============================================================
void wifi_update() {
    if (!wifi_initialized) return;
    
    // Check AP timeout
    if (ap_active && !ap_timeout_occurred) {
        unsigned long elapsed = millis() - ap_start_time;
        
        if (elapsed >= WIFI_AP_TIMEOUT_MS) {
            // Timeout reached
            if (WiFi.isConnected()) {
                // Connected to network, safe to disable AP
                wifi_stop_ap();
                WiFi.mode(WIFI_STA);  // Switch to STA only mode
            } else {
                // Not connected, keep AP active but mark timeout
                ap_timeout_occurred = true;
                Serial.println("[WIFI] AP timeout reached but no network connected - keeping AP active");
            }
        }
    }
    
    // Monitor STA connection status
    static bool was_connected = false;
    bool is_connected = WiFi.isConnected();
    
    if (is_connected && !was_connected) {
        current_wifi_status = WIFI_STATUS_CONNECTED;
        Serial.printf("[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
    } else if (!is_connected && was_connected) {
        current_wifi_status = ap_active ? WIFI_STATUS_AP_ONLY : WIFI_STATUS_DISCONNECTED;
        Serial.println("[WIFI] Lost connection");
        
        // If connection lost and AP is off, restart AP for recovery
        if (!ap_active) {
            Serial.println("[WIFI] Restarting AP for recovery...");
            WiFi.mode(WIFI_AP_STA);
            wifi_start_ap();
        }
    }
    
    was_connected = is_connected;
}

// ============================================================
// Getter Functions
// ============================================================
wifi_status_t wifi_get_status() {
    return current_wifi_status;
}

const char* wifi_get_status_str() {
    switch (current_wifi_status) {
        case WIFI_STATUS_CONNECTED:    return "Bagli";
        case WIFI_STATUS_CONNECTING:   return "Baglaniyor...";
        case WIFI_STATUS_AP_ONLY:      return "Sadece AP";
        default:                       return "Bagli Degil";
    }
}

String wifi_get_ip() {
    if (WiFi.isConnected()) {
        return WiFi.localIP().toString();
    }
    if (ap_active) {
        return WiFi.softAPIP().toString();
    }
    return "0.0.0.0";
}

String wifi_get_ap_ip() {
    if (ap_active) {
        return WiFi.softAPIP().toString();
    }
    return "-";
}

String wifi_get_sta_ip() {
    if (WiFi.isConnected()) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}

String wifi_get_ssid() {
    if (WiFi.isConnected()) {
        return WiFi.SSID();
    }
    return "";
}

String wifi_get_saved_ssid() {
    return saved_ssid;
}

int wifi_get_signal_percent() {
    if (!WiFi.isConnected()) return 0;
    
    int rssi = WiFi.RSSI();
    int percent = 2 * (rssi + 100);
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    return percent;
}

int wifi_get_ap_clients() {
    if (!ap_active) return 0;
    return WiFi.softAPgetStationNum();
}

bool wifi_is_connected() {
    return WiFi.isConnected();
}

bool wifi_has_saved_network() {
    return saved_ssid.length() > 0;
}

#endif // WIFI_HANDLER_H
