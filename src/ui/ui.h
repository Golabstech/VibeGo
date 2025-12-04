#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Disclaimer
void ui_Disclaimer_screen_init(void);
extern lv_obj_t * ui_Disclaimer;
extern lv_obj_t * ui_Disclaimer_Panel;
extern lv_obj_t * ui_Disclaimer_Title;
extern lv_obj_t * ui_Disclaimer_Text;
extern lv_obj_t * ui_Disclaimer_Button;
extern lv_obj_t * ui_Disclaimer_Button_Label;

// SCREEN: ui_Home
void ui_Home_screen_init(void);
extern lv_obj_t * ui_Home;
extern lv_obj_t * ui_Home_Container;
extern lv_obj_t * ui_Home_Left;
extern lv_obj_t * ui_Home_Logo;
extern lv_obj_t * ui_Home_Title;
extern lv_obj_t * ui_Home_Subtitle;
extern lv_obj_t * ui_Home_Pulse_Group;
extern lv_obj_t * ui_Home_Pulse_Ring;
extern lv_obj_t * ui_Home_Status_Pill;
extern lv_obj_t * ui_Home_Icon;
extern lv_obj_t * ui_Home_Status_Text;
extern lv_obj_t * ui_Home_Right;
extern lv_obj_t * ui_Home_Instructions;

// SCREEN: ui_Measuring
void ui_Measuring_screen_init(void);
extern lv_obj_t * ui_Measuring;
extern lv_obj_t * ui_Measuring_Gauge;
extern lv_obj_t * ui_Measuring_Text;

// SCREEN: ui_Result_Safe
void ui_Result_Safe_screen_init(void);
extern lv_obj_t * ui_Result_Safe;
extern lv_obj_t * ui_Result_Safe_Panel_Left;
extern lv_obj_t * ui_Result_Safe_Panel_Right;
extern lv_obj_t * ui_Result_Safe_Value;

// SCREEN: ui_Result_Danger
void ui_Result_Danger_screen_init(void);
extern lv_obj_t * ui_Result_Danger;
extern lv_obj_t * ui_Result_Danger_Panel_Left;
extern lv_obj_t * ui_Result_Danger_Panel_Right;
extern lv_obj_t * ui_Result_Danger_Value;

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
