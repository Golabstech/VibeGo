/**
 * ============================================================
 * VibeGo Kiosk - UI Definitions
 * Platform: JC2432W328 (320x240 Landscape)
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
#define UI_COLOR_BG_DARK      0x0F172A
#define UI_COLOR_PANEL        0x1E293B
#define UI_COLOR_PRIMARY      0x3B82F6
#define UI_COLOR_CYAN         0x06B6D4
#define UI_COLOR_GREEN        0x22C55E
#define UI_COLOR_RED          0xEF4444
#define UI_COLOR_ORANGE       0xF59E0B
#define UI_COLOR_TEXT_PRIMARY 0xFFFFFF
#define UI_COLOR_TEXT_MUTED   0x94A3B8
#define UI_COLOR_TEXT_BODY    0xCBD5E1

// ============================================================
// Screen Dimensions (Landscape)
// ============================================================
#define UI_SCREEN_WIDTH  320
#define UI_SCREEN_HEIGHT 240

// ============================================================
// Screen Objects
// ============================================================

// SCREEN: Splash
extern lv_obj_t * ui_Splash;
extern lv_obj_t * ui_Splash_Logo;
extern lv_obj_t * ui_Splash_Title;
extern lv_obj_t * ui_Splash_Subtitle;
extern lv_obj_t * ui_Splash_Progress;
extern lv_obj_t * ui_Splash_Version;

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
extern lv_obj_t * ui_Result_QR;
extern lv_obj_t * ui_Result_QR_Label;
extern lv_obj_t * ui_Result_QR_Title;

// ============================================================
// Function Prototypes
// ============================================================
void ui_Splash_screen_init(void);
void ui_Disclaimer_screen_init(void);
void ui_Home_screen_init(void);
void ui_Measuring_screen_init(void);
void ui_Result_screen_init(void);

void ui_init(void);

// Result screen helpers
void ui_show_result_safe(float bac_value);
void ui_show_result_danger(float bac_value);

// QR Code update (call when sponsor URL changes)
void ui_update_qr_code(const char* url);

// QR Title update (call when sponsor title changes)
void ui_update_qr_title(const char* title);

// Get measuring screen for simulation
lv_obj_t * ui_get_measuring_screen(void);

#ifdef __cplusplus
}
#endif

#endif // _VIBEGO_UI_H
