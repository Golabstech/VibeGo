/**
 * ============================================================
 * VibeGo - Brightness Control Module
 * ============================================================
 * Auto brightness using LDR sensor on GPIO34
 * ============================================================
 */

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <Arduino.h>

// ============================================================
// Configuration
// ============================================================
#ifndef LDR_PIN
  #define LDR_PIN 34
#endif

#define BRIGHTNESS_MIN       30      // Minimum brightness (dark room)
#define BRIGHTNESS_MAX       255     // Maximum brightness (bright light)
#define LDR_READ_INTERVAL_MS 500     // Read LDR every 500ms
#define BRIGHTNESS_SMOOTHING 0.3f    // Smoothing factor (0-1, lower = smoother)

// ============================================================
// State
// ============================================================
static bool brightness_auto_enabled = true;
static int current_brightness = BRIGHTNESS_MAX;
static int target_brightness = BRIGHTNESS_MAX;
static uint32_t last_ldr_read = 0;

// Callback to set display brightness
typedef void (*set_brightness_cb_t)(int brightness);
static set_brightness_cb_t brightness_callback = nullptr;

// ============================================================
// Functions
// ============================================================

/**
 * Set the callback function to control display brightness
 */
void brightness_set_callback(set_brightness_cb_t cb) {
    brightness_callback = cb;
}

/**
 * Initialize LDR pin
 */
void brightness_init() {
    pinMode(LDR_PIN, INPUT);
    analogSetAttenuation(ADC_11db);  // Full 3.3V range
    Serial.println("[BRIGHTNESS] Initialized on GPIO" + String(LDR_PIN));
}

/**
 * Read LDR value (0-4095)
 */
int brightness_read_ldr() {
    return analogRead(LDR_PIN);
}

/**
 * Map LDR value to brightness
 * 
 * Measured values on JC2432W328:
 * - Dark room:  ~0.8V  → ADC ~990  → Need LOW brightness
 * - Bright:     ~0.03V → ADC ~37   → Need HIGH brightness
 * 
 * So we need INVERTED mapping: Low ADC = Bright room = High brightness
 */
int brightness_ldr_to_level(int ldr_value) {
    // INVERTED: Low ADC value = bright environment = high display brightness
    // High ADC value = dark environment = low display brightness
    int brightness = map(ldr_value, 0, 4095, BRIGHTNESS_MAX, BRIGHTNESS_MIN);
    return constrain(brightness, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
}

/**
 * Update brightness (call in loop)
 */
void brightness_update() {
    if (!brightness_auto_enabled) return;
    if (!brightness_callback) return;
    
    uint32_t now = millis();
    if (now - last_ldr_read < LDR_READ_INTERVAL_MS) return;
    last_ldr_read = now;
    
    // Read LDR
    int ldr_value = brightness_read_ldr();
    target_brightness = brightness_ldr_to_level(ldr_value);
    
    // Smooth transition
    int diff = target_brightness - current_brightness;
    if (abs(diff) > 5) {  // Only update if significant change
        current_brightness = current_brightness + (int)(diff * BRIGHTNESS_SMOOTHING);
        
        // Apply brightness
        brightness_callback(current_brightness);
        
        // Debug (every 5 seconds)
        static uint32_t last_debug = 0;
        if (now - last_debug > 5000) {
            Serial.printf("[LDR] Raw: %d -> Brightness: %d\n", ldr_value, current_brightness);
            last_debug = now;
        }
    }
}

/**
 * Enable/disable auto brightness
 */
void brightness_set_auto(bool enabled) {
    brightness_auto_enabled = enabled;
    Serial.printf("[BRIGHTNESS] Auto mode: %s\n", enabled ? "ON" : "OFF");
}

/**
 * Check if auto brightness is enabled
 */
bool brightness_is_auto() {
    return brightness_auto_enabled;
}

/**
 * Manually set brightness (0-255)
 */
void brightness_set_manual(int level) {
    brightness_auto_enabled = false;
    current_brightness = constrain(level, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    
    if (brightness_callback) {
        brightness_callback(current_brightness);
    }
}

/**
 * Get current brightness level
 */
int brightness_get_level() {
    return current_brightness;
}

/**
 * Get current LDR reading (for debug/display)
 */
int brightness_get_ldr_raw() {
    return analogRead(LDR_PIN);
}

#endif // BRIGHTNESS_H
