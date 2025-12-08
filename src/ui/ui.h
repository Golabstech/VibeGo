/**
 * ============================================================
 * AlkoMetric / VibeGo Kiosk - UI Definitions
 * Adapted for JC2432W328 (320x240 Landscape)
 * ============================================================
 */

#ifndef _VIBEGO_UI_H
#define _VIBEGO_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// ============================================================
// Color Palette (DARK THEME)
// ============================================================
#define UI_COLOR_BG_DARK      0x0F172A   // Ana arka plan (koyu lacivert)
#define UI_COLOR_PANEL        0x1E293B   // Panel arka planı
#define UI_COLOR_PRIMARY      0x3B82F6   // Ana renk (mavi)
#define UI_COLOR_CYAN         0x06B6D4   // Vurgu rengi (cyan)
#define UI_COLOR_GREEN        0x22C55E   // Güvenli (yeşil)
#define UI_COLOR_RED          0xEF4444   // Tehlike (kırmızı)
#define UI_COLOR_ORANGE       0xF59E0B   // Uyarı (turuncu)
#define UI_COLOR_TEXT_PRIMARY 0xFFFFFF   // Ana metin (beyaz)
#define UI_COLOR_TEXT_MUTED   0x94A3B8   // Soluk metin
#define UI_COLOR_TEXT_BODY    0xCBD5E1   // Gövde metin

// ============================================================
// Screen Dimensions (Landscape)
// ============================================================
#define UI_SCREEN_WIDTH  320
#define UI_SCREEN_HEIGHT 240

// ============================================================
// Screen Objects
// ============================================================

// SCREEN: Disclaimer
extern lv_obj_t * ui_Disclaimer;
extern lv_obj_t * ui_Disclaimer_Panel;
extern lv_obj_t * ui_Disclaimer_Title;
extern lv_obj_t * ui_Disclaimer_Text;
extern lv_obj_t * ui_Disclaimer_Button;

// SCREEN: Home
extern lv_obj_t * ui_Home;
extern lv_obj_t * ui_Home_Logo;
extern lv_obj_t * ui_Home_Subtitle;
extern lv_obj_t * ui_Home_Status_Btn;
extern lv_obj_t * ui_Home_Instructions;

// SCREEN: Measuring
extern lv_obj_t * ui_Measuring;
extern lv_obj_t * ui_Measuring_Spinner;
extern lv_obj_t * ui_Measuring_Text;
extern lv_obj_t * ui_Measuring_Progress;

// SCREEN: Result
extern lv_obj_t * ui_Result;
extern lv_obj_t * ui_Result_Status_Label;
extern lv_obj_t * ui_Result_Value;
extern lv_obj_t * ui_Result_Unit;
extern lv_obj_t * ui_Result_Message;
extern lv_obj_t * ui_Result_NewTest_Btn;

// SCREEN: Settings (Service Mode Only)
extern lv_obj_t * ui_Settings;
extern lv_obj_t * ui_Settings_Wifi_Label;
extern lv_obj_t * ui_Settings_IP_Label;
extern lv_obj_t * ui_Settings_Brightness_Label;
extern lv_obj_t * ui_Settings_Version_Label;

// ============================================================
// Function Prototypes
// ============================================================
void ui_Disclaimer_screen_init(void);
void ui_Home_screen_init(void);
void ui_Measuring_screen_init(void);
void ui_Result_screen_init(void);
void ui_Settings_screen_init(void);

void ui_init(void);

// Result screen helpers
void ui_show_result_safe(float bac_value);
void ui_show_result_danger(float bac_value);

// Settings screen helpers (Service Mode)
void ui_settings_show(void);
void ui_settings_update_wifi(const char* status, const char* ip);
void ui_settings_update_brightness(int level, bool is_auto);

// Get measuring screen for simulation
lv_obj_t * ui_get_measuring_screen(void);

#ifdef __cplusplus
}
#endif

#endif // _VIBEGO_UI_H
