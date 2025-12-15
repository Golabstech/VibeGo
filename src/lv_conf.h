/**
 * @file lv_conf.h
 * Configuration file for LVGL v8.3.11
 * OPTIMIZED for VibeGo / ESP32-S3-WROOM-1 with PSRAM
 * 
 * Platform: Waveshare ESP32-S3-Touch-LCD-5 (1024x600)
 * Features:
 * - Full font set for large display
 * - PSRAM-backed memory
 * - QR code support
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0
#define LV_COLOR_SCREEN_TRANSP 0

/*=========================
   MEMORY SETTINGS
 *=========================*/
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
    #define LV_MEM_SIZE (128 * 1024U)   // 128KB for LVGL heap (PSRAM available)
    #define LV_MEM_ADR 0
    #define LV_MEM_POOL_INCLUDE <stdlib.h>
    #define LV_MEM_POOL_ALLOC malloc
#endif

/*====================
   HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif

// Display refresh rate
#define LV_DISP_DEF_REFR_PERIOD 30     // 33 FPS

/*================
   LOGGING
 *================*/
#define LV_USE_LOG 0                    // Disable logging to save flash

/*================
   FONT SETTINGS
 *================*/
// Font quality: 4 BPP for smooth anti-aliased text
#define LV_FONT_MONTSERRAT_10 1         // Small hints
#define LV_FONT_MONTSERRAT_12 1         // Small text
#define LV_FONT_MONTSERRAT_14 1         // Body text  
#define LV_FONT_MONTSERRAT_16 1         // Buttons (small)
#define LV_FONT_MONTSERRAT_20 1         // Headers
#define LV_FONT_MONTSERRAT_24 1         // Subtitles (new for 1024x600)
#define LV_FONT_MONTSERRAT_28 1         // Titles (new for 1024x600)
#define LV_FONT_MONTSERRAT_32 1         // Large buttons (new for 1024x600)
#define LV_FONT_MONTSERRAT_40 1         // Extra large (new for 1024x600)
#define LV_FONT_MONTSERRAT_48 1         // Big numbers on Result screen

// Disabled fonts
#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_30 1
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0

// Font compression (saves ~30-50%)
#define LV_USE_FONT_COMPRESSED 1

// Subpixel rendering (disable to save memory)
#define LV_USE_FONT_SUBPX 0

// Default font
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*================
   THEME
 *================*/
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 1         // Dark theme
#define LV_THEME_DEFAULT_GROW 0         // Disable grow animation on press

/*================
   TEXT
 *================*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8      // UTF-8 encoding
#define LV_TXT_LINE_BREAK_LONG_LEN 0    // Disable long text breaking
#define LV_TXT_COLOR_CMD "#"

/*================
   WIDGETS - Core (enabled)
 *================*/
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BTN 1
#define LV_USE_BTNMATRIX 0
#define LV_USE_CANVAS 1
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_IMG 1
#define LV_USE_LABEL 1
#define LV_USE_LINE 0
#define LV_USE_ROLLER 0
#define LV_USE_SLIDER 0
#define LV_USE_SWITCH 0
#define LV_USE_TABLE 0
#define LV_USE_TEXTAREA 0

/*================
   WIDGETS - Extra (all disabled)
 *================*/
#define LV_USE_ANIMIMG 0
#define LV_USE_CALENDAR 0
#define LV_USE_CHART 0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN 0
#define LV_USE_KEYBOARD 0
#define LV_USE_LED 0
#define LV_USE_LIST 0
#define LV_USE_MENU 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX 0
#define LV_USE_SPAN 0
#define LV_USE_SPINBOX 0
#define LV_USE_SPINNER 1                // Keep for measuring screen
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0

/*================
   ANIMATIONS
 *================*/
#define LV_USE_ANIMATION 1              // Enable for smooth transitions
#define LV_USE_SHADOW 1                 // Keep shadows for premium look
#define LV_USE_BLEND_MODES 0            // Disable blend modes

/*================
   GPU / DRAWING
 *================*/
#define LV_USE_GPU 0
#define LV_USE_GPU_SDL 0

/*================
   OTHER
 *================*/
#define LV_USE_QRCODE 1
#define LV_USE_SNAPSHOT 0
#define LV_USE_MONKEY 0

#endif /*LV_CONF_H*/
