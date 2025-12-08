/**
 * ============================================================
 * AlkoMetric / VibeGo Kiosk - UI Implementation
 * Adapted for JC2432W328 (320x240 Landscape) - DARK THEME
 * ============================================================
 */

#include "ui.h"
#include <stdio.h>
#include <string.h>

// ============================================================
// Variable Definitions
// ============================================================

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

// Result Screen (dynamic - safe or danger)
lv_obj_t * ui_Result;
lv_obj_t * ui_Result_Status_Label;
lv_obj_t * ui_Result_Value;
lv_obj_t * ui_Result_Unit;
lv_obj_t * ui_Result_Message;
lv_obj_t * ui_Result_NewTest_Btn;

// Settings Screen (Service Mode Only)
lv_obj_t * ui_Settings;
lv_obj_t * ui_Settings_Wifi_Label;
lv_obj_t * ui_Settings_IP_Label;
lv_obj_t * ui_Settings_Brightness_Label;
lv_obj_t * ui_Settings_Version_Label;

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

// ============================================================
// SCREEN: Disclaimer (Yasal Uyarı) - LANDSCAPE
// ============================================================
void ui_Disclaimer_screen_init(void) {
    ui_Disclaimer = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Disclaimer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Disclaimer, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Main Panel
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

    // Title
    ui_Disclaimer_Title = lv_label_create(ui_Disclaimer_Panel);
    lv_label_set_text(ui_Disclaimer_Title, LV_SYMBOL_WARNING " YASAL UYARI");
    lv_obj_set_style_text_color(ui_Disclaimer_Title, lv_color_hex(UI_COLOR_ORANGE), 0);
    lv_obj_set_style_text_font(ui_Disclaimer_Title, &lv_font_montserrat_20, 0);
    lv_obj_align(ui_Disclaimer_Title, LV_ALIGN_TOP_MID, 0, 0);

    // Text
    ui_Disclaimer_Text = lv_label_create(ui_Disclaimer_Panel);
    lv_label_set_long_mode(ui_Disclaimer_Text, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Disclaimer_Text, 270);
    lv_label_set_text(ui_Disclaimer_Text, 
        "Bu cihaz sadece eglence amaclidir.\n"
        "Sonuclar tibbi/yasal gecerlilik tasimaz.\n"
        "Alkol aldiysaniz arac kullanmayiniz!");
    lv_obj_set_style_text_color(ui_Disclaimer_Text, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Disclaimer_Text, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(ui_Disclaimer_Text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(ui_Disclaimer_Text, LV_ALIGN_CENTER, 0, -5);

    // Accept Button
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
// SCREEN: Home (Ana Ekran) - LANDSCAPE
// ============================================================
void ui_Home_screen_init(void) {
    ui_Home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Home, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Left side - Logo and button
    lv_obj_t * left_panel = lv_obj_create(ui_Home);
    lv_obj_set_size(left_panel, 180, 220);
    lv_obj_align(left_panel, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_opa(left_panel, 0, 0);
    lv_obj_set_style_border_width(left_panel, 0, 0);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);
    
    // Logo
    ui_Home_Logo = lv_label_create(left_panel);
    lv_label_set_text(ui_Home_Logo, "VIBEGO");
    lv_obj_set_style_text_font(ui_Home_Logo, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_Home_Logo, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_set_style_text_letter_space(ui_Home_Logo, 2, 0);
    lv_obj_align(ui_Home_Logo, LV_ALIGN_TOP_MID, 0, 10);

    // Subtitle
    ui_Home_Subtitle = lv_label_create(left_panel);
    lv_label_set_text(ui_Home_Subtitle, "Check Your Vibe");
    lv_obj_set_style_text_font(ui_Home_Subtitle, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Home_Subtitle, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Home_Subtitle, LV_ALIGN_TOP_MID, 0, 45);

    // Pulse Ring (background glow)
    lv_obj_t * pulse_ring = lv_obj_create(left_panel);
    lv_obj_set_size(pulse_ring, 150, 55);
    lv_obj_align(pulse_ring, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_color(pulse_ring, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_opa(pulse_ring, 60, 0);
    lv_obj_set_style_radius(pulse_ring, 27, 0);
    lv_obj_set_style_border_width(pulse_ring, 0, 0);

    // Start Button
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

    // Right side - Instructions Panel
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
        "1. Pipet al\n"
        "2. Nefes al\n"
        "3. Ufle\n"
        "4. Bekle");
    lv_obj_set_style_text_color(ui_Home_Instructions, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Home_Instructions, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Home_Instructions, LV_ALIGN_CENTER, 0, 10);
}

// ============================================================
// SCREEN: Measuring (Ölçüm) - LANDSCAPE
// ============================================================
void ui_Measuring_screen_init(void) {
    ui_Measuring = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Measuring, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Measuring, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Left side - Spinner
    ui_Measuring_Spinner = lv_spinner_create(ui_Measuring, 1000, 60);
    lv_obj_set_size(ui_Measuring_Spinner, 100, 100);
    lv_obj_align(ui_Measuring_Spinner, LV_ALIGN_LEFT_MID, 40, -10);
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 8, LV_PART_MAIN);

    // Right side - Text
    ui_Measuring_Text = lv_label_create(ui_Measuring);
    lv_label_set_text(ui_Measuring_Text, "Olcum Yapiliyor...");
    lv_obj_set_style_text_font(ui_Measuring_Text, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_Measuring_Text, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_align(ui_Measuring_Text, LV_ALIGN_RIGHT_MID, -30, -30);

    lv_obj_t * hint = lv_label_create(ui_Measuring);
    lv_label_set_text(hint, "Lutfen ufleyin...");
    lv_obj_set_style_text_color(hint, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_align(hint, LV_ALIGN_RIGHT_MID, -50, 10);

    // Progress bar
    ui_Measuring_Progress = lv_bar_create(ui_Measuring);
    lv_obj_set_size(ui_Measuring_Progress, 280, 15);
    lv_obj_align(ui_Measuring_Progress, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_bar_set_range(ui_Measuring_Progress, 0, 100);
    lv_bar_set_value(ui_Measuring_Progress, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ui_Measuring_Progress, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_Measuring_Progress, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui_Measuring_Progress, 7, LV_PART_MAIN);
    lv_obj_set_style_radius(ui_Measuring_Progress, 7, LV_PART_INDICATOR);
}

// ============================================================
// SCREEN: Result (Sonuç) - LANDSCAPE
// ============================================================
void ui_Result_screen_init(void) {
    ui_Result = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Result, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Result, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Left panel - Result
    lv_obj_t * left_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(left_panel, 180, 200);
    lv_obj_align(left_panel, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_bg_color(left_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(left_panel, 220, 0);
    lv_obj_set_style_radius(left_panel, 16, 0);
    lv_obj_set_style_border_width(left_panel, 0, 0);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);

    // Status Label
    ui_Result_Status_Label = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_OK " GUVENLI");
    lv_obj_set_style_text_font(ui_Result_Status_Label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_GREEN), 0);
    lv_obj_align(ui_Result_Status_Label, LV_ALIGN_TOP_MID, 0, 15);

    // BAC Value
    ui_Result_Value = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Value, "0.00");
    lv_obj_set_style_text_font(ui_Result_Value, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_GREEN), 0);
    lv_obj_align(ui_Result_Value, LV_ALIGN_CENTER, 0, 0);

    // Unit
    ui_Result_Unit = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Unit, "promil");
    lv_obj_set_style_text_font(ui_Result_Unit, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Result_Unit, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_align(ui_Result_Unit, LV_ALIGN_CENTER, 0, 35);

    // Right panel - Message & Button
    lv_obj_t * right_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(right_panel, 105, 200);
    lv_obj_align(right_panel, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_bg_color(right_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(right_panel, 180, 0);
    lv_obj_set_style_radius(right_panel, 16, 0);
    lv_obj_set_style_border_width(right_panel, 0, 0);
    lv_obj_set_style_pad_all(right_panel, 10, 0);
    lv_obj_clear_flag(right_panel, LV_OBJ_FLAG_SCROLLABLE);

    // Message
    ui_Result_Message = lv_label_create(right_panel);
    lv_label_set_long_mode(ui_Result_Message, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Result_Message, 85);
    lv_label_set_text(ui_Result_Message, "Iyi yolculuklar!");
    lv_obj_set_style_text_align(ui_Result_Message, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(ui_Result_Message, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Result_Message, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_align(ui_Result_Message, LV_ALIGN_TOP_MID, 0, 10);

    // New Test Button
    ui_Result_NewTest_Btn = lv_btn_create(right_panel);
    lv_obj_set_size(ui_Result_NewTest_Btn, 85, 50);
    lv_obj_align(ui_Result_NewTest_Btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ui_Result_NewTest_Btn, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_radius(ui_Result_NewTest_Btn, 12, 0);
    lv_obj_add_event_cb(ui_Result_NewTest_Btn, ui_event_new_test_btn, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(ui_Result_NewTest_Btn);
    lv_label_set_text(btn_label, "YENI\nTEST");
    lv_obj_set_style_text_align(btn_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(btn_label, lv_color_hex(UI_COLOR_TEXT_PRIMARY), 0);
    lv_obj_center(btn_label);
}

// ============================================================
// Result Screen Helpers
// ============================================================

void ui_show_result_safe(float bac_value) {
    // Update status
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_OK " GUVENLI");
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_GREEN), 0);
    
    // Update value
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", bac_value);
    lv_label_set_text(ui_Result_Value, buf);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_GREEN), 0);
    
    // Update message
    lv_label_set_text(ui_Result_Message, "Iyi yolculuklar!\nGuvenli surusler.");
    
    // Show screen
    lv_scr_load_anim(ui_Result, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}

void ui_show_result_danger(float bac_value) {
    // Update status
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_CLOSE " TEHLIKE");
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_RED), 0);
    
    // Update value
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", bac_value);
    lv_label_set_text(ui_Result_Value, buf);
    lv_obj_set_style_text_color(ui_Result_Value, lv_color_hex(UI_COLOR_RED), 0);
    
    // Update message
    lv_label_set_text(ui_Result_Message, "ARAC KULLANMA!\nTaksi cagir.");
    
    // Show screen
    lv_scr_load_anim(ui_Result, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}

lv_obj_t * ui_get_measuring_screen(void) {
    return ui_Measuring;
}

// ============================================================
// SCREEN: Settings (Service Mode Only)
// ============================================================
static void ui_event_settings_back(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

void ui_Settings_screen_init(void) {
    ui_Settings = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Settings, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Settings, lv_color_hex(UI_COLOR_BG_DARK), 0);

    // Title
    lv_obj_t * title = lv_label_create(ui_Settings);
    lv_label_set_text(title, LV_SYMBOL_SETTINGS " AYARLAR");
    lv_obj_set_style_text_color(title, lv_color_hex(UI_COLOR_CYAN), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Service Mode indicator
    lv_obj_t * service_badge = lv_label_create(ui_Settings);
    lv_label_set_text(service_badge, "SERVIS MODU");
    lv_obj_set_style_text_color(service_badge, lv_color_hex(UI_COLOR_ORANGE), 0);
    lv_obj_set_style_text_font(service_badge, &lv_font_montserrat_12, 0);
    lv_obj_align(service_badge, LV_ALIGN_TOP_RIGHT, -10, 12);

    // Info Panel
    lv_obj_t * panel = lv_obj_create(ui_Settings);
    lv_obj_set_size(panel, 300, 160);
    lv_obj_align(panel, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_bg_color(panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(panel, 220, 0);
    lv_obj_set_style_radius(panel, 12, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_pad_all(panel, 15, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    // WiFi Status
    lv_obj_t * wifi_icon = lv_label_create(panel);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_color(wifi_icon, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_align(wifi_icon, LV_ALIGN_TOP_LEFT, 0, 0);

    ui_Settings_Wifi_Label = lv_label_create(panel);
    lv_label_set_text(ui_Settings_Wifi_Label, "WiFi: Bagli Degil");
    lv_obj_set_style_text_color(ui_Settings_Wifi_Label, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Settings_Wifi_Label, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Settings_Wifi_Label, LV_ALIGN_TOP_LEFT, 25, 2);

    // IP Address
    ui_Settings_IP_Label = lv_label_create(panel);
    lv_label_set_text(ui_Settings_IP_Label, "IP: -");
    lv_obj_set_style_text_color(ui_Settings_IP_Label, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(ui_Settings_IP_Label, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Settings_IP_Label, LV_ALIGN_TOP_LEFT, 25, 22);

    // Brightness
    lv_obj_t * bright_icon = lv_label_create(panel);
    lv_label_set_text(bright_icon, LV_SYMBOL_IMAGE);
    lv_obj_set_style_text_color(bright_icon, lv_color_hex(UI_COLOR_ORANGE), 0);
    lv_obj_align(bright_icon, LV_ALIGN_TOP_LEFT, 0, 50);

    ui_Settings_Brightness_Label = lv_label_create(panel);
    lv_label_set_text(ui_Settings_Brightness_Label, "Parlaklik: 100% (Oto)");
    lv_obj_set_style_text_color(ui_Settings_Brightness_Label, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_set_style_text_font(ui_Settings_Brightness_Label, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Settings_Brightness_Label, LV_ALIGN_TOP_LEFT, 25, 52);

    // Version
    ui_Settings_Version_Label = lv_label_create(panel);
    lv_label_set_text(ui_Settings_Version_Label, "Firmware: v1.0.0");
    lv_obj_set_style_text_color(ui_Settings_Version_Label, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(ui_Settings_Version_Label, &lv_font_montserrat_12, 0);
    lv_obj_align(ui_Settings_Version_Label, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    // OTA Hostname
    lv_obj_t * ota_label = lv_label_create(panel);
    lv_label_set_text(ota_label, "OTA: vibego.local:3232");
    lv_obj_set_style_text_color(ota_label, lv_color_hex(UI_COLOR_TEXT_MUTED), 0);
    lv_obj_set_style_text_font(ota_label, &lv_font_montserrat_12, 0);
    lv_obj_align(ota_label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // Back Button
    lv_obj_t * back_btn = lv_btn_create(ui_Settings);
    lv_obj_set_size(back_btn, 100, 35);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_border_color(back_btn, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_border_width(back_btn, 1, 0);
    lv_obj_set_style_radius(back_btn, 17, 0);
    lv_obj_add_event_cb(back_btn, ui_event_settings_back, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " GERI");
    lv_obj_set_style_text_font(back_label, &lv_font_montserrat_12, 0);
    lv_obj_center(back_label);
}

// Settings helpers
void ui_settings_show(void) {
    lv_scr_load_anim(ui_Settings, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
}

void ui_settings_update_wifi(const char* status, const char* ip) {
    char buf[64];
    snprintf(buf, sizeof(buf), "WiFi: %s", status);
    lv_label_set_text(ui_Settings_Wifi_Label, buf);
    
    snprintf(buf, sizeof(buf), "IP: %s", ip);
    lv_label_set_text(ui_Settings_IP_Label, buf);
}

void ui_settings_update_brightness(int level, bool is_auto) {
    char buf[32];
    int percent = (level * 100) / 255;
    snprintf(buf, sizeof(buf), "Parlaklik: %d%% (%s)", percent, is_auto ? "Oto" : "Manuel");
    lv_label_set_text(ui_Settings_Brightness_Label, buf);
}

// ============================================================
// UI Initialization (DARK THEME)
// ============================================================
void ui_init(void) {
    // Initialize DARK theme
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, 
        lv_color_hex(UI_COLOR_PRIMARY),    // Primary color
        lv_color_hex(UI_COLOR_CYAN),       // Secondary color
        true,                               // Dark mode = true
        &lv_font_montserrat_14);
    lv_disp_set_theme(dispp, theme);

    // Initialize all screens
    ui_Disclaimer_screen_init();
    ui_Home_screen_init();
    ui_Measuring_screen_init();
    ui_Result_screen_init();
    ui_Settings_screen_init();

    // Load first screen
    lv_disp_load_scr(ui_Disclaimer);
}
