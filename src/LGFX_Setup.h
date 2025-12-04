#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

class LGFX : public lgfx::LGFX_Device
{
#ifdef WOKWI
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
#else
    lgfx::v1::Panel_RGB     _panel_instance;
    lgfx::v1::Bus_RGB       _bus_instance;
    lgfx::v1::Touch_GT911   _touch_instance;
#endif

public:
    LGFX(void)
    {
#ifdef WOKWI
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read  = 16000000;
            cfg.pin_sclk = 12;
            cfg.pin_mosi = 11;
            cfg.pin_miso = 13;
            cfg.pin_dc   = 9;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs   = 10;
            cfg.pin_rst  = 14;
            cfg.pin_busy = -1;
            cfg.panel_width  = 240;
            cfg.panel_height = 320;
            cfg.offset_x     = 0;
            cfg.offset_y     = 0;
            _panel_instance.config(cfg);
        }
#else
        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;

            // Pin configuration for JC4827W543 (Standard Sunton/Guitton ESP32-S3 RGB Pinout)
            
            cfg.pin_d0  = 8; // B0
            cfg.pin_d1  = 3; // B1
            cfg.pin_d2  = 46; // B2
            cfg.pin_d3  = 9; // B3
            cfg.pin_d4  = 1; // B4
            
            cfg.pin_d5  = 5; // G0
            cfg.pin_d6  = 6; // G1
            cfg.pin_d7  = 7; // G2
            cfg.pin_d8  = 15; // G3
            cfg.pin_d9  = 16; // G4
            cfg.pin_d10 = 4; // G5
            
            cfg.pin_d11 = 45; // R0
            cfg.pin_d12 = 48; // R1
            cfg.pin_d13 = 47; // R2
            cfg.pin_d14 = 21; // R3
            cfg.pin_d15 = 14; // R4

            cfg.pin_henable = 40;
            cfg.pin_vsync   = 41;
            cfg.pin_hsync   = 39;
            cfg.pin_pclk    = 42;
            cfg.freq_write  = 16000000;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 8;
            cfg.hsync_pulse_width = 4;
            cfg.hsync_back_porch  = 8;
            
            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 8;
            cfg.vsync_pulse_width = 4;
            cfg.vsync_back_porch  = 8;

            cfg.pclk_idle_high    = 1;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.memory_width  = 480;
            cfg.memory_height = 272;
            cfg.panel_width   = 480;
            cfg.panel_height  = 272;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            _panel_instance.config(cfg);
        }

        {
            auto cfg = _touch_instance.config();
            cfg.x_min      = 0;
            cfg.x_max      = 479;
            cfg.y_min      = 0;
            cfg.y_max      = 271;
            cfg.pin_int    = -1; // Interrupt pin
            cfg.pin_rst    = -1; // Reset pin
            cfg.bus_shared = true; // Shared with other I2C devices?
            cfg.offset_rotation = 0;
            
            // I2C pins for Touch (Commonly 19/20 or similar on S3)
            cfg.i2c_port   = 1;
            cfg.i2c_addr   = 0x5D; // Or 0x14
            cfg.pin_sda    = 19;
            cfg.pin_scl    = 20;
            cfg.freq       = 400000;

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
#endif
        setPanel(&_panel_instance);
    }
};
