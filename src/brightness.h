/**
 * ============================================================
 * VibeGo - Brightness Control Module
 * ============================================================
 * Manual brightness control via Web/Serial
 * (LDR Auto-Brightness Disabled for Waveshare Display)
 * ============================================================
 */

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <Arduino.h>

// ============================================================
// Configuration
// ============================================================
#define BRIGHTNESS_MIN       10      // Minimum brightness
#define BRIGHTNESS_MAX       255     // Maximum brightness
#define BRIGHTNESS_SMOOTHING 0.3f    // Smoothing factor

// ============================================================
// State
// ============================================================
static bool brightness_auto_enabled = false; // Always manual
static int current_brightness = BRIGHTNESS_MAX;
static int target_brightness = BRIGHTNESS_MAX;

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
 * Initialize (Manual Mode)
 */
void brightness_init() {
    Serial.println("[BRIGHTNESS] Initialized (Manual Mode - No LDR)");
}

/**
 * Update brightness (Simple smooth transition for manual changes)
 */
void brightness_update() {
    // In manual mode, we just smoothly transition to target if changed
    if (!brightness_callback) return;
    
    int diff = target_brightness - current_brightness;
    if (abs(diff) > 2) { 
        current_brightness = current_brightness + (int)(diff * BRIGHTNESS_SMOOTHING);
        // Snap to target if close
        if (abs(target_brightness - current_brightness) < 3) {
            current_brightness = target_brightness;
        }
        brightness_callback(current_brightness);
    }
}

/**
 * Enable/disable auto brightness (Stub)
 */
void brightness_set_auto(bool enabled) {
    brightness_auto_enabled = false; // Force false
    Serial.println("[BRIGHTNESS] Auto mode not supported on this hardware");
}

/**
 * Check if auto brightness is enabled
 */
bool brightness_is_auto() {
    return false;
}

/**
 * Manually set brightness (0-255)
 */
void brightness_set_manual(int level) {
    target_brightness = constrain(level, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    Serial.printf("[BRIGHTNESS] Set target: %d\n", target_brightness);
}

/**
 * Get current brightness level
 */
int brightness_get_level() {
    return current_brightness;
}

/**
 * Get raw LDR value (Stub)
 */
int brightness_get_ldr_raw() {
    return 0;
}

#endif // BRIGHTNESS_H
