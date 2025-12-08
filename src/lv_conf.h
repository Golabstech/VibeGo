/**
 * @file lv_conf.h
 * Configuration file for v8.3.11
 * OPTIMIZED for ESP32 + WiFiManager
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
    /* Reduced from 64KB to 32KB for WiFiManager compatibility */
    #define LV_MEM_SIZE (32 * 1024U)
    #define LV_MEM_ADR 0
#endif

/*====================
   HAL SETTINGS
 *====================*/

#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif

/*================
   FONT USAGE
   Reduced font set to save RAM
 *================*/
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_28 0   /* Disabled */
#define LV_FONT_MONTSERRAT_32 0   /* Disabled */
#define LV_FONT_MONTSERRAT_48 1   /* Keep for big numbers */

/*================
   WIDGETS
   Disable unused widgets
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
#define LV_USE_SPINBOX 0
#define LV_USE_TABVIEW 0
#define LV_USE_TILEVIEW 0
#define LV_USE_WIN 0
#define LV_USE_SPAN 0

/*================
   OTHERS
 *================*/
#define LV_USE_QRCODE 0   /* Disabled to save memory */

#endif /*LV_CONF_H*/
