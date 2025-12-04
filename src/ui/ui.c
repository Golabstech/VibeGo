#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Disclaimer;
lv_obj_t * ui_Disclaimer_Panel;
lv_obj_t * ui_Disclaimer_Title;
lv_obj_t * ui_Disclaimer_Text;
lv_obj_t * ui_Disclaimer_Button;
lv_obj_t * ui_Disclaimer_Button_Label;

lv_obj_t * ui_Home;
lv_obj_t * ui_Home_Container;
lv_obj_t * ui_Home_Left;
lv_obj_t * ui_Home_Logo;
lv_obj_t * ui_Home_Title;
lv_obj_t * ui_Home_Subtitle;
lv_obj_t * ui_Home_Pulse_Group;
lv_obj_t * ui_Home_Pulse_Ring;
lv_obj_t * ui_Home_Status_Pill;
lv_obj_t * ui_Home_Icon;
lv_obj_t * ui_Home_Status_Text;
lv_obj_t * ui_Home_Right;
lv_obj_t * ui_Home_Instructions;

lv_obj_t * ui_Measuring;
lv_obj_t * ui_Measuring_Gauge;
lv_obj_t * ui_Measuring_Text;

lv_obj_t * ui_Result_Safe;
lv_obj_t * ui_Result_Safe_Panel_Left;
lv_obj_t * ui_Result_Safe_Panel_Right;
lv_obj_t * ui_Result_Safe_Value;

lv_obj_t * ui_Result_Danger;
lv_obj_t * ui_Result_Danger_Panel_Left;
lv_obj_t * ui_Result_Danger_Panel_Right;
lv_obj_t * ui_Result_Danger_Value;

///////////////////// ANIMATIONS ////////////////////
void Pulse_Animation(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, TargetObject);
    lv_anim_set_values(&a, 100, 255); // Scale: 1.0 to 2.5
    lv_anim_set_time(&a, 2000);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom); // Use zoom for scaling
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);

    lv_anim_set_values(&a, 255, 0); // Opacity: 100% to 0%
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_style_bg_opa);
    lv_anim_start(&a);
}

///////////////////// FUNCTIONS ////////////////////
void ui_event_Disclaimer_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

void ui_event_Home_Status_Pill(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Measuring, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

void ui_event_Measuring_Gauge(lv_event_t * e)
{
    // Simulation: Click to finish measuring
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        // Randomly go to Safe or Danger for demo
        if(rand() % 2 == 0)
            lv_scr_load_anim(ui_Result_Safe, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
        else
            lv_scr_load_anim(ui_Result_Danger, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

void ui_event_New_Test_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_Disclaimer, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
    }
}

///////////////////// SCREENS ////////////////////

void ui_Disclaimer_screen_init(void)
{
    ui_Disclaimer = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Disclaimer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Disclaimer, lv_color_hex(0x0F172A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Disclaimer_Panel = lv_obj_create(ui_Disclaimer);
    lv_obj_set_width(ui_Disclaimer_Panel, 600);
    lv_obj_set_height(ui_Disclaimer_Panel, 350);
    lv_obj_set_align(ui_Disclaimer_Panel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Disclaimer_Panel, lv_color_hex(0x1E293B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Disclaimer_Panel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Disclaimer_Panel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Disclaimer_Panel, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Disclaimer_Panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Disclaimer_Panel, 24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Disclaimer_Title = lv_label_create(ui_Disclaimer_Panel);
    lv_obj_set_width(ui_Disclaimer_Title, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Disclaimer_Title, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_Disclaimer_Title, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Disclaimer_Title, "YASAL UYARI");
    lv_obj_set_style_text_color(ui_Disclaimer_Title, lv_color_hex(0x06B6D4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Disclaimer_Title, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Disclaimer_Text = lv_label_create(ui_Disclaimer_Panel);
    lv_obj_set_width(ui_Disclaimer_Text, 550);
    lv_obj_set_height(ui_Disclaimer_Text, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_Disclaimer_Text, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Disclaimer_Text, "Bu cihaz (VibeGo) yalnizca eglence ve bilgilendirme amacli tasarlanmistir.\nCihazin olcum sonuclari; ortam kosullari, sensor durumu ve kullanim sekline gore degisiklik gosterebilir.\nSonuclarin yasal, tibbi veya adli bir gecerliligi yoktur.\nSurus yeterliliginin tespiti icin kullanilamaz.\nAlkol aldiysaniz, lutfen arac kullanmayiniz.");
    lv_obj_set_style_text_color(ui_Disclaimer_Text, lv_color_hex(0xCBD5E1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Disclaimer_Text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Disclaimer_Text, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Disclaimer_Button = lv_btn_create(ui_Disclaimer_Panel);
    lv_obj_set_width(ui_Disclaimer_Button, 250);
    lv_obj_set_height(ui_Disclaimer_Button, 60);
    lv_obj_set_align(ui_Disclaimer_Button, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(ui_Disclaimer_Button, lv_color_hex(0x3B82F6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Disclaimer_Button, lv_color_hex(0x06B6D4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Disclaimer_Button, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Disclaimer_Button, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Disclaimer_Button, ui_event_Disclaimer_Button, LV_EVENT_ALL, NULL);

    ui_Disclaimer_Button_Label = lv_label_create(ui_Disclaimer_Button);
    lv_obj_set_align(ui_Disclaimer_Button_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Disclaimer_Button_Label, "KABUL EDIYORUM");
    lv_obj_set_style_text_font(ui_Disclaimer_Button_Label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_Home_screen_init(void)
{
    ui_Home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Home, lv_color_hex(0x0F172A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_Container = lv_obj_create(ui_Home);
    lv_obj_set_size(ui_Home_Container, 800, 480);
    lv_obj_set_style_bg_opa(ui_Home_Container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_Home_Container, 0, LV_PART_MAIN);
    lv_obj_set_layout(ui_Home_Container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ui_Home_Container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Home_Container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(ui_Home_Container, 60, 0);

    // Left Column
    ui_Home_Left = lv_obj_create(ui_Home_Container);
    lv_obj_set_size(ui_Home_Left, 400, 400);
    lv_obj_set_style_bg_opa(ui_Home_Left, 0, 0);
    lv_obj_set_style_border_width(ui_Home_Left, 0, 0);
    lv_obj_set_flex_flow(ui_Home_Left, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Home_Left, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_Home_Logo = lv_label_create(ui_Home_Left);
    lv_label_set_text(ui_Home_Logo, "VIBEGO");
    lv_obj_set_style_text_font(ui_Home_Logo, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(ui_Home_Logo, lv_color_white(), 0);
    lv_obj_set_style_text_letter_space(ui_Home_Logo, 4, 0);

    ui_Home_Subtitle = lv_label_create(ui_Home_Left);
    lv_label_set_text(ui_Home_Subtitle, "Check Your Vibe. Ready to Go?");
    lv_obj_set_style_text_font(ui_Home_Subtitle, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(ui_Home_Subtitle, lv_color_hex(0x94A3B8), 0);
    lv_obj_set_style_pad_bottom(ui_Home_Subtitle, 40, 0);

    // Pulse Group
    ui_Home_Pulse_Group = lv_obj_create(ui_Home_Left);
    lv_obj_set_size(ui_Home_Pulse_Group, 300, 100);
    lv_obj_set_style_bg_opa(ui_Home_Pulse_Group, 0, 0);
    lv_obj_set_style_border_width(ui_Home_Pulse_Group, 0, 0);

    ui_Home_Pulse_Ring = lv_obj_create(ui_Home_Pulse_Group);
    lv_obj_set_size(ui_Home_Pulse_Ring, 200, 60);
    lv_obj_set_align(ui_Home_Pulse_Ring, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Home_Pulse_Ring, lv_color_hex(0x3B82F6), 0);
    lv_obj_set_style_radius(ui_Home_Pulse_Ring, 30, 0);
    // Pulse animation would be applied here

    ui_Home_Status_Pill = lv_obj_create(ui_Home_Pulse_Group);
    lv_obj_set_size(ui_Home_Status_Pill, 220, 60);
    lv_obj_set_align(ui_Home_Status_Pill, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Home_Status_Pill, lv_color_hex(0x0F172A), 0);
    lv_obj_set_style_border_color(ui_Home_Status_Pill, lv_color_hex(0x3B82F6), 0);
    lv_obj_set_style_border_width(ui_Home_Status_Pill, 2, 0);
    lv_obj_set_style_radius(ui_Home_Status_Pill, 30, 0);
    lv_obj_add_event_cb(ui_Home_Status_Pill, ui_event_Home_Status_Pill, LV_EVENT_ALL, NULL);

    ui_Home_Status_Text = lv_label_create(ui_Home_Status_Pill);
    lv_obj_set_align(ui_Home_Status_Text, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_Status_Text, "UFLEMEYE BASLA");
    lv_obj_set_style_text_color(ui_Home_Status_Text, lv_color_white(), 0);

    // Right Column (Instructions)
    ui_Home_Right = lv_obj_create(ui_Home_Container);
    lv_obj_set_size(ui_Home_Right, 260, 300);
    lv_obj_set_style_bg_color(ui_Home_Right, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_bg_opa(ui_Home_Right, 200, 0);
    lv_obj_set_style_radius(ui_Home_Right, 24, 0);
    lv_obj_set_style_border_width(ui_Home_Right, 0, 0);
    lv_obj_set_style_border_side(ui_Home_Right, LV_BORDER_SIDE_LEFT, 0);
    lv_obj_set_style_border_color(ui_Home_Right, lv_color_hex(0x3B82F6), 0);
    lv_obj_set_style_border_width(ui_Home_Right, 4, 0);
    
    ui_Home_Instructions = lv_label_create(ui_Home_Right);
    lv_label_set_text(ui_Home_Instructions, "NASIL KULLANILIR?\n\n1. Pipet Alin\n2. Sensore Takin\n3. Derin Nefes Ufleyin\n4. Sonucu Gorun");
    lv_obj_set_width(ui_Home_Instructions, 220);
    lv_obj_set_align(ui_Home_Instructions, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_Home_Instructions, lv_color_hex(0xCBD5E1), 0);
}

void ui_Measuring_screen_init(void)
{
    ui_Measuring = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Measuring, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Measuring, lv_color_hex(0x0F172A), 0);

    ui_Measuring_Gauge = lv_spinner_create(ui_Measuring, 1000, 60);
    lv_obj_set_size(ui_Measuring_Gauge, 250, 250);
    lv_obj_set_align(ui_Measuring_Gauge, LV_ALIGN_CENTER);
    lv_obj_set_style_arc_color(ui_Measuring_Gauge, lv_color_hex(0x3B82F6), LV_PART_INDICATOR);
    lv_obj_add_event_cb(ui_Measuring_Gauge, ui_event_Measuring_Gauge, LV_EVENT_ALL, NULL);

    ui_Measuring_Text = lv_label_create(ui_Measuring);
    lv_label_set_text(ui_Measuring_Text, "Analyzing...\nPlease Blow");
    lv_obj_set_align(ui_Measuring_Text, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(ui_Measuring_Text, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(ui_Measuring_Text, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(ui_Measuring_Text, lv_color_white(), 0);
}

void ui_Result_Safe_screen_init(void)
{
    ui_Result_Safe = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Result_Safe, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Result_Safe, lv_color_hex(0x0F172A), 0);

    // Left Panel
    ui_Result_Safe_Panel_Left = lv_obj_create(ui_Result_Safe);
    lv_obj_set_size(ui_Result_Safe_Panel_Left, 340, 380);
    lv_obj_set_align(ui_Result_Safe_Panel_Left, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Result_Safe_Panel_Left, 50);
    lv_obj_set_style_bg_color(ui_Result_Safe_Panel_Left, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_bg_opa(ui_Result_Safe_Panel_Left, 200, 0);
    lv_obj_set_style_radius(ui_Result_Safe_Panel_Left, 24, 0);

    lv_obj_t * safe_label = lv_label_create(ui_Result_Safe_Panel_Left);
    lv_label_set_text(safe_label, "SAFE TO DRIVE");
    lv_obj_set_align(safe_label, LV_ALIGN_TOP_MID);
    lv_obj_set_y(safe_label, 30);
    lv_obj_set_style_text_color(safe_label, lv_color_hex(0x22C55E), 0);
    lv_obj_set_style_text_font(safe_label, &lv_font_montserrat_28, 0);

    ui_Result_Safe_Value = lv_label_create(ui_Result_Safe_Panel_Left);
    lv_label_set_text(ui_Result_Safe_Value, "0.12");
    lv_obj_set_align(ui_Result_Safe_Value, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(ui_Result_Safe_Value, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(ui_Result_Safe_Value, lv_color_white(), 0);

    // Right Panel (QR)
    ui_Result_Safe_Panel_Right = lv_obj_create(ui_Result_Safe);
    lv_obj_set_size(ui_Result_Safe_Panel_Right, 240, 380);
    lv_obj_set_align(ui_Result_Safe_Panel_Right, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_Result_Safe_Panel_Right, -50);
    lv_obj_set_style_bg_color(ui_Result_Safe_Panel_Right, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_bg_opa(ui_Result_Safe_Panel_Right, 200, 0);
    lv_obj_set_style_radius(ui_Result_Safe_Panel_Right, 24, 0);

    lv_obj_t * qr = lv_qrcode_create(ui_Result_Safe_Panel_Right, 160, lv_color_hex(0x000000), lv_color_hex(0xffffff));
    lv_qrcode_update(qr, "https://m.uber.com/ul/", strlen("https://m.uber.com/ul/"));
    lv_obj_set_align(qr, LV_ALIGN_TOP_MID);
    lv_obj_set_y(qr, 30);

    lv_obj_t * btn = lv_btn_create(ui_Result_Safe_Panel_Right);
    lv_obj_set_width(btn, 200);
    lv_obj_set_align(btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(btn, -20);
    lv_obj_t * btn_lbl = lv_label_create(btn);
    lv_label_set_text(btn_lbl, "NEW TEST");
    lv_obj_set_align(btn_lbl, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(btn, ui_event_New_Test_Button, LV_EVENT_ALL, NULL);
}

void ui_Result_Danger_screen_init(void)
{
    ui_Result_Danger = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Result_Danger, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Result_Danger, lv_color_hex(0x0F172A), 0);

    // Left Panel
    ui_Result_Danger_Panel_Left = lv_obj_create(ui_Result_Danger);
    lv_obj_set_size(ui_Result_Danger_Panel_Left, 340, 380);
    lv_obj_set_align(ui_Result_Danger_Panel_Left, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Result_Danger_Panel_Left, 50);
    lv_obj_set_style_bg_color(ui_Result_Danger_Panel_Left, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_bg_opa(ui_Result_Danger_Panel_Left, 200, 0);
    lv_obj_set_style_radius(ui_Result_Danger_Panel_Left, 24, 0);
    lv_obj_set_style_border_color(ui_Result_Danger_Panel_Left, lv_color_hex(0xEF4444), 0);
    lv_obj_set_style_border_width(ui_Result_Danger_Panel_Left, 2, 0);

    lv_obj_t * danger_label = lv_label_create(ui_Result_Danger_Panel_Left);
    lv_label_set_text(danger_label, "DO NOT DRIVE");
    lv_obj_set_align(danger_label, LV_ALIGN_TOP_MID);
    lv_obj_set_y(danger_label, 30);
    lv_obj_set_style_text_color(danger_label, lv_color_hex(0xEF4444), 0);
    lv_obj_set_style_text_font(danger_label, &lv_font_montserrat_28, 0);

    ui_Result_Danger_Value = lv_label_create(ui_Result_Danger_Panel_Left);
    lv_label_set_text(ui_Result_Danger_Value, "1.45");
    lv_obj_set_align(ui_Result_Danger_Value, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(ui_Result_Danger_Value, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(ui_Result_Danger_Value, lv_color_hex(0xEF4444), 0);

    // Right Panel (QR)
    ui_Result_Danger_Panel_Right = lv_obj_create(ui_Result_Danger);
    lv_obj_set_size(ui_Result_Danger_Panel_Right, 240, 380);
    lv_obj_set_align(ui_Result_Danger_Panel_Right, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_Result_Danger_Panel_Right, -50);
    lv_obj_set_style_bg_color(ui_Result_Danger_Panel_Right, lv_color_hex(0x1E293B), 0);
    lv_obj_set_style_bg_opa(ui_Result_Danger_Panel_Right, 200, 0);
    lv_obj_set_style_radius(ui_Result_Danger_Panel_Right, 24, 0);
    lv_obj_set_style_border_color(ui_Result_Danger_Panel_Right, lv_color_hex(0xEF4444), 0);
    lv_obj_set_style_border_width(ui_Result_Danger_Panel_Right, 2, 0);

    lv_obj_t * qr = lv_qrcode_create(ui_Result_Danger_Panel_Right, 160, lv_color_hex(0x000000), lv_color_hex(0xffffff));
    lv_qrcode_update(qr, "https://m.uber.com/ul/", strlen("https://m.uber.com/ul/"));
    lv_obj_set_align(qr, LV_ALIGN_TOP_MID);
    lv_obj_set_y(qr, 30);

    lv_obj_t * btn = lv_btn_create(ui_Result_Danger_Panel_Right);
    lv_obj_set_width(btn, 200);
    lv_obj_set_align(btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(btn, -20);
    lv_obj_t * btn_lbl = lv_label_create(btn);
    lv_label_set_text(btn_lbl, "NEW TEST");
    lv_obj_set_align(btn_lbl, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(btn, ui_event_New_Test_Button, LV_EVENT_ALL, NULL);
}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    ui_Disclaimer_screen_init();
    ui_Home_screen_init();
    ui_Measuring_screen_init();
    ui_Result_Safe_screen_init();
    ui_Result_Danger_screen_init();

    lv_disp_load_scr(ui_Disclaimer);
}
