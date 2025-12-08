/**
 * ============================================================
 * AlkoMetric Kiosk - LGFX Display Configuration
 * Platform: JC2432W328 (ESP32 + ST7789 + CST820)
 * ============================================================
 * 
 * Display: 2.8" TFT LCD, 240x320, ST7789 (SPI)
 * Touch: CST820 Capacitive (I2C)
 * 
 * Pin Mapping:
 * - Display SPI: MOSI=13, SCLK=14, CS=15, DC=2, BL=27
 * - Touch I2C: SDA=33, SCL=32, RST=25, INT=21
 * ============================================================
 */

#ifndef LGFX_SETUP_H
#define LGFX_SETUP_H

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Wire.h>

// ============================================================
// Pin Definitions (Can be overridden via build_flags)
// ============================================================

// Display Pins (ST7789 - SPI)
#ifndef TFT_MOSI
  #define TFT_MOSI 13
#endif
#ifndef TFT_SCLK
  #define TFT_SCLK 14
#endif
#ifndef TFT_CS
  #define TFT_CS 15
#endif
#ifndef TFT_DC
  #define TFT_DC 2
#endif
#ifndef TFT_RST
  #define TFT_RST -1  // Not connected or internal
#endif
#ifndef TFT_BL
  #define TFT_BL 27
#endif

// Touch Pins (CST820 - I2C)
#ifndef TOUCH_SDA
  #define TOUCH_SDA 33
#endif
#ifndef TOUCH_SCL
  #define TOUCH_SCL 32
#endif
#ifndef TOUCH_RST
  #define TOUCH_RST 25
#endif
#ifndef TOUCH_INT
  #define TOUCH_INT 21
#endif

// Screen Resolution (Landscape Mode)
#ifndef SCREEN_WIDTH
  #define SCREEN_WIDTH 320
#endif
#ifndef SCREEN_HEIGHT
  #define SCREEN_HEIGHT 240
#endif

// ============================================================
// LGFX Device Class
// ============================================================
class LGFX : public lgfx::LGFX_Device {
private:
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_CST816S _touch_instance;  // CST820 compatible with CST816S driver

public:
  LGFX(void) {
    // ========================================
    // SPI Bus Configuration
    // ========================================
    {
      auto cfg = _bus_instance.config();
      
      cfg.spi_host = VSPI_HOST;        // VSPI = SPI3
      cfg.spi_mode = 0;                // SPI Mode 0
      cfg.freq_write = 40000000;       // 40MHz write speed
      cfg.freq_read = 16000000;        // 16MHz read speed
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      
      // Pin assignments
      cfg.pin_sclk = TFT_SCLK;
      cfg.pin_mosi = TFT_MOSI;
      cfg.pin_miso = -1;               // Not used for display
      cfg.pin_dc = TFT_DC;
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // ========================================
    // Panel Configuration (ST7789)
    // ========================================
    {
      auto cfg = _panel_instance.config();
      
      cfg.pin_cs = TFT_CS;
      cfg.pin_rst = TFT_RST;
      cfg.pin_busy = -1;
      
      cfg.panel_width = SCREEN_WIDTH;
      cfg.panel_height = SCREEN_HEIGHT;
      cfg.memory_width = SCREEN_WIDTH;
      cfg.memory_height = SCREEN_HEIGHT;
      
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;             // JC2432W328: invert = false for correct colors
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      
      _panel_instance.config(cfg);
    }

    // ========================================
    // Backlight Configuration (PWM)
    // ========================================
    {
      auto cfg = _light_instance.config();
      
      cfg.pin_bl = TFT_BL;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 0;
      
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    // ========================================
    // Touch Configuration (CST820/CST816S)
    // ========================================
    {
      auto cfg = _touch_instance.config();
      
      cfg.x_min = 0;
      cfg.x_max = SCREEN_WIDTH - 1;
      cfg.y_min = 0;
      cfg.y_max = SCREEN_HEIGHT - 1;
      
      cfg.pin_int = TOUCH_INT;
      cfg.pin_rst = TOUCH_RST;
      
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      
      // I2C Configuration
      cfg.i2c_port = 1;                // I2C port 1
      cfg.i2c_addr = 0x15;             // CST820 default address
      cfg.pin_sda = TOUCH_SDA;
      cfg.pin_scl = TOUCH_SCL;
      cfg.freq = 400000;               // 400kHz I2C
      
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }

  // ========================================
  // Utility Methods
  // ========================================
  
  /**
   * Set backlight brightness (0-255)
   */
  void setBacklight(uint8_t brightness) {
    setBrightness(brightness);
  }

  /**
   * Turn backlight on (full brightness)
   */
  void backlightOn() {
    setBrightness(255);
  }

  /**
   * Turn backlight off
   */
  void backlightOff() {
    setBrightness(0);
  }
};

#endif // LGFX_SETUP_H
