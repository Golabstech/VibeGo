/**
 * ============================================================
 * VibeGo Kiosk - UI Implementation
 * Platform: JC2432W328 (320x240 Landscape) - DARK THEME
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

// Result Screen
lv_obj_t * ui_Result;
lv_obj_t * ui_Result_Status_Label;
lv_obj_t * ui_Result_Value;
lv_obj_t * ui_Result_Unit;
lv_obj_t * ui_Result_Message;
lv_obj_t * ui_Result_NewTest_Btn;

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
        "Bu cihaz sadece eglence amaclidir.\n"
        "Sonuclar tibbi/yasal gecerlilik tasimaz.\n"
        "Alkol aldiysaniz arac kullanmayiniz!");
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
        "1. Pipet al\n"
        "2. Nefes al\n"
        "3. Ufle\n"
        "4. Bekle");
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

    ui_Measuring_Spinner = lv_spinner_create(ui_Measuring, 1000, 60);
    lv_obj_set_size(ui_Measuring_Spinner, 100, 100);
    lv_obj_align(ui_Measuring_Spinner, LV_ALIGN_LEFT_MID, 40, -10);
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_CYAN), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ui_Measuring_Spinner, lv_color_hex(UI_COLOR_PANEL), LV_PART_MAIN);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(ui_Measuring_Spinner, 8, LV_PART_MAIN);

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
// SCREEN: Result
// ============================================================
void ui_Result_screen_init(void) {
    ui_Result = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Result, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Result, lv_color_hex(UI_COLOR_BG_DARK), 0);

    lv_obj_t * left_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(left_panel, 180, 200);
    lv_obj_align(left_panel, LV_ALIGN_LEFT_MID, 15, 0);
    lv_obj_set_style_bg_color(left_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(left_panel, 220, 0);
    lv_obj_set_style_radius(left_panel, 16, 0);
    lv_obj_set_style_border_width(left_panel, 0, 0);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);

    ui_Result_Status_Label = lv_label_create(left_panel);
    lv_label_set_text(ui_Result_Status_Label, LV_SYMBOL_OK " GUVENLI");
    lv_obj_set_style_text_font(ui_Result_Status_Label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(ui_Result_Status_Label, lv_color_hex(UI_COLOR_GREEN), 0);
    lv_obj_align(ui_Result_Status_Label, LV_ALIGN_TOP_MID, 0, 15);

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

    lv_obj_t * right_panel = lv_obj_create(ui_Result);
    lv_obj_set_size(right_panel, 105, 200);
    lv_obj_align(right_panel, LV_ALIGN_RIGHT_MID, -15, 0);
    lv_obj_set_style_bg_color(right_panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_bg_opa(right_panel, 180, 0);
    lv_obj_set_style_radius(right_panel, 16, 0);
    lv_obj_set_style_border_width(right_panel, 0, 0);
    lv_obj_set_style_pad_all(right_panel, 10, 0);
    lv_obj_clear_flag(right_panel, LV_OBJ_FLAG_SCROLLABLE);

    ui_Result_Message = lv_label_create(right_panel);
    lv_label_set_long_mode(ui_Result_Message, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_Result_Message, 85);
    lv_label_set_text(ui_Result_Message, "Iyi yolculuklar!");
    lv_obj_set_style_text_align(ui_Result_Message, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(ui_Result_Message, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(ui_Result_Message, lv_color_hex(UI_COLOR_TEXT_BODY), 0);
    lv_obj_align(ui_Result_Message, LV_ALIGN_TOP_MID, 0, 10);

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

    ui_Disclaimer_screen_init();
    ui_Home_screen_init();
    ui_Measuring_screen_init();
    ui_Result_screen_init();

    lv_disp_load_scr(ui_Disclaimer);
}
