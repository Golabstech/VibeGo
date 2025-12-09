/**
 * ============================================================
 * VibeGo Kiosk - UI Implementation
 * Platform: JC2432W328 (320x240 Landscape) - DARK THEME
 * ============================================================
 */

#include "ui.h"
#include <stdio.h>
#include <string.h>

// External logo image
LV_IMG_DECLARE(golabs_logo);

// ============================================================
// Variable Definitions
// ============================================================

// Splash Screen
lv_obj_t * ui_Splash;
lv_obj_t * ui_Splash_Logo;
lv_obj_t * ui_Splash_Title;
lv_obj_t * ui_Splash_Subtitle;
lv_obj_t * ui_Splash_Progress;
lv_obj_t * ui_Splash_Version;
static lv_timer_t * splash_timer = NULL;
static int splash_progress = 0;

// Disclaimer Screen
lv_obj_t * ui_Disclaimer;
lv_obj_t * ui_Disclaimer_Panel;
lv_obj_t * ui_Disclaimer_Title;
lv_obj_t * ui_Disclaimer_Text;
lv_obj_t * ui_Disclaimer_Button;

// Home Screen
lv_obj_t * ui_Home;
lv_obj_t * ui_Home_Logo;
lv_obj_t * ui_Home_Subtitle;
lv_obj_t * ui_Home_Status_Btn;
lv_obj_t * ui_Home_Instructions;

// Measuring Screen
lv_obj_t * ui_Measuring;
lv_obj_t * ui_Measuring_Spinner;
lv_obj_t * ui_Measuring_Text;
lv_obj_t * ui_Measuring_Progress;

// Result Screen
lv_obj_t * ui_Result;
lv_obj_t * ui_Result_Status_Label;
lv_obj_t * ui_Result_Value;
lv_obj_t * ui_Result_Unit;
lv_obj_t * ui_Result_Message;
lv_obj_t * ui_Result_NewTest_Btn;
lv_obj_t * ui_Result_QR;
lv_obj_t * ui_Result_QR_Label;
lv_obj_t * ui_Result_QR_Title;

// ============================================================
// Event Handlers
// ============================================================
static void ui_event_disclaimer_btn(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

static void ui_event_start_btn(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Measuring, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

static void ui_event_new_test_btn(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Disclaimer, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

// Splash screen timer callback
static void splash_timer_cb(lv_timer_t * timer) {
    splash_progress += 5;
    if (splash_progress > 100) splash_progress = 100;
    
    lv_bar_set_value(ui_Splash_Progress, splash_progress, LV_ANIM_ON);
    
    if (splash_progress >= 100) {
        lv_timer_del(timer);
        splash_timer = NULL;
        lv_scr_load_anim(ui_Disclaimer, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

// ============================================================
// SCREEN: Splash
// ============================================================
void ui_Splash_screen_init(void) {
    ui_Splash = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Splash, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Splash, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Logo image (Golabs) - positioned in upper-center area
    ui_Splash_Logo = lv_img_create(ui_Splash);
    lv_img_set_src(ui_Splash_Logo, &golabs_logo);
    lv_obj_align(ui_Splash_Logo, LV_ALIGN_CENTER, 0, -40);
    // Note: Logo is displayed as-is (RGB565 with dark background baked in)

    // Title removed - logo already contains "GOLABS" text
    ui_Splash_Title = NULL;
    
    // Subtitle - Developer credit
    ui_Splash_Subtitle = lv_label_create(ui_Splash);
    lv_label_set_text(ui_Splash_Subtitle, "Developed by Golabs A.S.");
    lv_obj_set_style_text_font(ui_Splash_Subtitle, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Splash_Subtitle, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Splash_Subtitle, LV_ALIGN_CENTER, 0, 45);

    // Progress bar
    ui_Splash_Progress = lv_bar_create(ui_Splash);
    lv_obj_set_size(ui_Splash_Progress, 200, 8);
    lv_obj_align(ui_Splash_Progress, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_bar_set_range(ui_Splash_Progress, 0, 100);
    lv_bar_set_value(ui_Splash_Progress, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ui_Splash_Progress, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_Splash_Progress, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui_Splash_Progress, 4, LV_PART_MAIN);
    lv_obj_set_style_radius(ui_Splash_Progress, 4, LV_PART_INDICATOR);

    // Version
    ui_Splash_Version = lv_label_create(ui_Splash);
    lv_label_set_text(ui_Splash_Version, "v1.4.0");
    lv_obj_set_style_text_font(ui_Splash_Version, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui_Splash_Version, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Splash_Version, LV_ALIGN_BOTTOM_MID, 0, -30);

    // Start progress timer (every 100ms, total 3 seconds for 100%)
    splash_progress = 0;
    splash_timer = lv_timer_create(splash_timer_cb, 150, NULL);
}

// ============================================================
// SCREEN: Disclaimer
// ============================================================
void ui_Disclaimer_screen_init(void) {
    ui_Disclaimer = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Disclaimer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Disclaimer, lv_color_hex(UI_COLOR_BG_DARK), 0);

    ui_Disclaimer_Panel = lv_obj_create(ui_Disclaimer);
    lv_obj_set_size(ui_Disclaimer_Panel, 300, 200);
    lv_obj_align(ui_Disclaimer_Panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(ui_Disclaimer_Panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(ui_Disclaimer_Panel, 240, 0);
    lv_obj_set_style_border_color(ui_Disclaimer_Panel, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_border_opa(ui_Disclaimer_Panel, 100, 0);
    lv_obj_set_style_border_width(ui_Disclaimer_Panel, 1, 0);
    lv_obj_set_style_radius(ui_Disclaimer_Panel, 16, 0);
    lv_obj_set_style_pad_all(ui_Disclaimer_Panel, 15, 0);
    lv_obj_clear_flag(ui_Disclaimer_Panel, LV_OBJ_FLAG_SCROLLABLE);

    ui_Disclaimer_Title = lv_label_create(ui_Disclaimer_Panel);
    lv_label_set_text(ui_Disclaimer_Title, LV_SYMBOL_WARNING " YASAL UYARI");
    lv_obj_set_style_text_color(ui_Disclaimer_Title, lv_color_hex(UI_COLOR_ORANGE), 0);
    lv_obj_set_style_text_font(ui_Disclaimer_Title, &lv_font_montserrat_20, 0);
    lv_obj_align(ui_Disclaimer_Title, LV_ALIGN_TOP_MID, 0, 0);

    ui_Disclaimer_Text = lv_label_create(ui_Disclaimer_Panel);
    lv_label_set_long_mode(ui_Disclaimer_Text, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Disclaimer_Text, 270);
    lv_label_set_text(ui_Disclaimer_Text, 
        "DIKKAT: Bu cihaz yasal veya tibbi\n"
        "olcum yapmaz. Sonuclar tahminidir.\n"
        "Alkol aldiysaniz ARAC KULLANMAYIN!");
    lv_obj_set_style_text_color(ui_Disclaimer_Text, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Disclaimer_Text, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(ui_Disclaimer_Text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(ui_Disclaimer_Text, LV_ALIGN_CENTER, 0, -5);

    ui_Disclaimer_Button = lv_btn_create(ui_Disclaimer_Panel);
    lv_obj_set_size(ui_Disclaimer_Button, 200, 40);
    lv_obj_align(ui_Disclaimer_Button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ui_Disclaimer_Button, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_grad_color(ui_Disclaimer_Button, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_bg_grad_dir(ui_Disclaimer_Button, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_radius(ui_Disclaimer_Button, 20, 0);
    lv_obj_set_style_shadow_width(ui_Disclaimer_Button, 10, 0);
    lv_obj_set_style_shadow_color(ui_Disclaimer_Button, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_shadow_opa(ui_Disclaimer_Button, 80, 0);
    lv_obj_add_event_cb(ui_Disclaimer_Button, ui_event_disclaimer_btn, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(ui_Disclaimer_Button);
    lv_label_set_text(btn_label, "KABUL EDIYORUM");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(btn_label, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_center(btn_label);
}

// ============================================================
// SCREEN: Home
// ============================================================
void ui_Home_screen_init(void) {
    ui_Home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Home, lv_color_hex(UI_COLOR_BG_DARK), 0);

    lv_obj_t * left_panel = lv_obj_create(ui_Home);
    lv_obj_set_size(left_panel, 180, 220);
    lv_obj_align(left_panel, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_opa(left_panel, 0, 0);
    lv_obj_set_style_border_width(left_panel, 0, 0);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);
    
    ui_Home_Logo = lv_label_create(left_panel);
    lv_label_set_text(ui_Home_Logo, "VIBEGO");
    lv_obj_set_style_text_font(ui_Home_Logo, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_Home_Logo, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_set_style_text_letter_space(ui_Home_Logo, 2, 0);
    lv_obj_align(ui_Home_Logo, LV_ALIGN_TOP_MID, 0, 10);

    ui_Home_Subtitle = lv_label_create(left_panel);
    lv_label_set_text(ui_Home_Subtitle, "Check Your Vibe");
    lv_obj_set_style_text_font(ui_Home_Subtitle, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Home_Subtitle, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Home_Subtitle, LV_ALIGN_TOP_MID, 0, 45);

    lv_obj_t * pulse_ring = lv_obj_create(left_panel);
    lv_obj_set_size(pulse_ring, 150, 55);
    lv_obj_align(pulse_ring, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_color(pulse_ring, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_opa(pulse_ring, 60, 0);
    lv_obj_set_style_radius(pulse_ring, 27, 0);
    lv_obj_set_style_border_width(pulse_ring, 0, 0);

    ui_Home_Status_Btn = lv_btn_create(left_panel);
    lv_obj_set_size(ui_Home_Status_Btn, 160, 50);
    lv_obj_align(ui_Home_Status_Btn, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_color(ui_Home_Status_Btn, lv_color_hex(UI_COLOR_BG_DARK), 0);
    lv_obj_set_style_border_color(ui_Home_Status_Btn, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_border_width(ui_Home_Status_Btn, 2, 0);
    lv_obj_set_style_radius(ui_Home_Status_Btn, 25, 0);
    lv_obj_add_event_cb(ui_Home_Status_Btn, ui_event_start_btn, LV_EVENT_CLICKED, NULL);

    lv_obj_t * status_label = lv_label_create(ui_Home_Status_Btn);
    lv_label_set_text(status_label, LV_SYMBOL_PLAY " TESTE BASLA");
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_14, 0);
    lv_obj_center(status_label);

    lv_obj_t * right_panel = lv_obj_create(ui_Home);
    lv_obj_set_size(right_panel, 115, 180);
    lv_obj_align(right_panel, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_style_bg_color(right_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(right_panel, 200, 0);
    lv_obj_set_style_radius(right_panel, 12, 0);
    lv_obj_set_style_border_color(right_panel, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_border_width(right_panel, 2, 0);
    lv_obj_set_style_border_side(right_panel, LV_BORDER_SIDE_LEFT, 0);
    lv_obj_set_style_pad_all(right_panel, 10, 0);
    lv_obj_clear_flag(right_panel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * instr_title = lv_label_create(right_panel);
    lv_label_set_text(instr_title, "KULLANIM");
    lv_obj_set_style_text_color(instr_title, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_font(instr_title, &lv_font_montserrat_12, 0);
    lv_obj_align(instr_title, LV_ALIGN_TOP_MID, 0, 0);

    ui_Home_Instructions = lv_label_create(right_panel);
    lv_label_set_text(ui_Home_Instructions, 
        "1. Pipet alin\n"
        "2. Yerlestirin\n"
        "3. Nefes alin\n"
        "4. 5sn ufleyin\n"
        "5. Bekleyin");
    lv_obj_set_style_text_color(ui_Home_Instructions, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Home_Instructions, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Home_Instructions, LV_ALIGN_CENTER, 0, 10);
}

// ============================================================
// SCREEN: Measuring
// ============================================================
void ui_Measuring_screen_init(void) {
    ui_Measuring = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Measuring, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Measuring, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Spinner on LEFT side
    ui_Measuring_Spinner = lv_spinner_create(ui_Measuring, 1000, 60);
    lv_obj_set_size(ui_Measuring_Spinner, 100, 100);
    lv_obj_set_pos(ui_Measuring_Spinner, 30, 60);  // x=30, y=60
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 10, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 10, LV_PART_MAIN);

    // Main text on RIGHT side - "OLCULUYOR..."
    ui_Measuring_Text = lv_label_create(ui_Measuring);
    lv_label_set_text(ui_Measuring_Text, "OLCULUYOR...");
    lv_obj_set_style_text_font(ui_Measuring_Text, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_Measuring_Text, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_set_style_text_letter_space(ui_Measuring_Text, 1, 0);
    lv_obj_set_pos(ui_Measuring_Text, 150, 80);  // x=150, y=80

    // Hint text below main text
    lv_obj_t * hint = lv_label_create(ui_Measuring);
    lv_label_set_text(hint, "Uflemeye Devam Edin");
    lv_obj_set_style_text_color(hint, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(hint, 150, 115);  // x=150, y=115

    // Progress bar at BOTTOM
    ui_Measuring_Progress = lv_bar_create(ui_Measuring);
    lv_obj_set_size(ui_Measuring_Progress, 280, 18);
    lv_obj_align(ui_Measuring_Progress, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_bar_set_range(ui_Measuring_Progress, 0, 100);
    lv_bar_set_value(ui_Measuring_Progress, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ui_Measuring_Progress, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_Measuring_Progress, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui_Measuring_Progress, 9, LV_PART_MAIN);
    lv_obj_set_style_radius(ui_Measuring_Progress, 9, LV_PART_INDICATOR);
    lv_obj_set_style_anim_time(ui_Measuring_Progress, 300, LV_PART_MAIN);
}

// ============================================================
// SCREEN: Result
// ============================================================

// Default QR URL - can be changed from web panel
#define DEFAULT_QR_URL "https://vibego.app/go"

void ui_Result_screen_init(void) {
    ui_Result = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Result, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Result, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // LEFT PANEL - BAC Value
    lv_obj_t * left_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(left_panel, 150, 200);
    lv_obj_align(left_panel, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_color(left_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(left_panel, 220, 0);
    lv_obj_set_style_radius(left_panel, 16, 0);
    lv_obj_set_style_border_width(left_panel, 0, 0);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);

    ui_Result_Status_Label = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_OK " GUVENLI");
    lv_obj_set_style_text_font(ui_Result_Status_Label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_GREEN), 0);
    lv_obj_align(ui_Result_Status_Label, LV_ALIGN_TOP_MID, 0, 12);

    ui_Result_Value = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Value, "0.00");
    lv_obj_set_style_text_font(ui_Result_Value, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_GREEN), 0);
    lv_obj_align(ui_Result_Value, LV_ALIGN_CENTER, 0, 0);

    ui_Result_Unit = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Unit, "promil");
    lv_obj_set_style_text_font(ui_Result_Unit, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Result_Unit, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Result_Unit, LV_ALIGN_CENTER, 0, 35);

    ui_Result_Message = lv_label_create(left_panel);
    lv_label_set_long_mode(ui_Result_Message, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Result_Message, 130);
    lv_label_set_text(ui_Result_Message, "Iyi yolculuklar!");
    lv_obj_set_style_text_align(ui_Result_Message, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(ui_Result_Message, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui_Result_Message, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Result_Message, LV_ALIGN_BOTTOM_MID, 0, -8);

    // RIGHT PANEL - QR Code + Button
    lv_obj_t * right_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(right_panel, 140, 200);
    lv_obj_align(right_panel, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_style_bg_color(right_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(right_panel, 180, 0);
    lv_obj_set_style_radius(right_panel, 16, 0);
    lv_obj_set_style_border_width(right_panel, 0, 0);
    lv_obj_set_style_pad_all(right_panel, 8, 0);
    lv_obj_clear_flag(right_panel, LV_OBJ_FLAG_SCROLLABLE);

    // QR Code Title (dynamic, can be changed from web panel)
    ui_Result_QR_Title = lv_label_create(right_panel);
    lv_label_set_long_mode(ui_Result_QR_Title, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Result_QR_Title, 120);  // Max width before wrap
    lv_label_set_text(ui_Result_QR_Title, "TAKSI CAG");
    lv_obj_set_style_text_font(ui_Result_QR_Title, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui_Result_QR_Title, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_align(ui_Result_QR_Title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(ui_Result_QR_Title, LV_ALIGN_TOP_MID, 0, 0);

    // QR Code
    #if LV_USE_QRCODE
    ui_Result_QR = lv_qrcode_create(right_panel, 80, 
        lv_color_hex(0x000000), lv_color_hex(0xFFFFFF));
    lv_qrcode_update(ui_Result_QR, DEFAULT_QR_URL, strlen(DEFAULT_QR_URL));
    lv_obj_align(ui_Result_QR, LV_ALIGN_CENTER, 0, -5);
    lv_obj_set_style_border_color(ui_Result_QR, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(ui_Result_QR, 3, 0);
    #endif

    // QR Hint
    ui_Result_QR_Label = lv_label_create(right_panel);
    lv_label_set_text(ui_Result_QR_Label, "Tara & Git");
    lv_obj_set_style_text_font(ui_Result_QR_Label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui_Result_QR_Label, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Result_QR_Label, LV_ALIGN_CENTER, 0, 48);

    // New Test Button
    ui_Result_NewTest_Btn = lv_btn_create(right_panel);
    lv_obj_set_size(ui_Result_NewTest_Btn, 110, 30);
    lv_obj_align(ui_Result_NewTest_Btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ui_Result_NewTest_Btn, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_radius(ui_Result_NewTest_Btn, 15, 0);
    lv_obj_add_event_cb(ui_Result_NewTest_Btn, ui_event_new_test_btn, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(ui_Result_NewTest_Btn);
    lv_label_set_text(btn_label, LV_SYMBOL_REFRESH " YENI TEST");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(btn_label, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_center(btn_label);
}

// ============================================================
// Result Screen Helpers
// ============================================================

/**
 * Update QR code with new URL
 * Call this when sponsor URL changes from web panel
 */
void ui_update_qr_code(const char* url) {
    #if LV_USE_QRCODE
    if (ui_Result_QR != NULL && url != NULL) {
        lv_qrcode_update(ui_Result_QR, url, strlen(url));
    }
    #endif
}

/**
 * Update QR title text
 * Call this when sponsor title changes from web panel
 */
void ui_update_qr_title(const char* title) {
    if (ui_Result_QR_Title != NULL && title != NULL) {
        lv_label_set_text(ui_Result_QR_Title, title);
    }
}

void ui_show_result_safe(float bac_value) {
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_OK " GUVENLI");
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_GREEN), 0);
    
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", bac_value);
    lv_label_set_text(ui_Result_Value, buf);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_GREEN), 0);
    
    lv_label_set_text(ui_Result_Message, "Iyi yolculuklar!\nGuvenli surusler.");
    lv_scr_load_anim(ui_Result, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}

void ui_show_result_danger(float bac_value) {
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_CLOSE " TEHLIKE");
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_RED), 0);
    
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", bac_value);
    lv_label_set_text(ui_Result_Value, buf);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_RED), 0);
    
    lv_label_set_text(ui_Result_Message, "ARAC KULLANMA!\nTaksi cagir.");
    lv_scr_load_anim(ui_Result, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}

lv_obj_t * ui_get_measuring_screen(void) {
    return ui_Measuring;
}

// ============================================================
// UI Initialization
// ============================================================
void ui_init(void) {
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, 
        lv_color_hex(UI_COLOR_PRIMARY),
        lv_color_hex(UI_COLOR_CYAN),
        true,  // Dark mode
        &lv_font_montserrat_14);
    lv_disp_set_theme(dispp, theme);

    ui_Splash_screen_init();
    ui_Disclaimer_screen_init();
    ui_Home_screen_init();
    ui_Measuring_screen_init();
    ui_Result_screen_init();

    // Start with Splash screen
    lv_disp_load_scr(ui_Splash);
}
