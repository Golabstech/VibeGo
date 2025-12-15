# 📘 ESP32-S3-Touch-LCD-5 Technical & Development Guide

## 1. General Description

- **Product:** ESP32-S3-Touch-LCD-5  
- **Size:** 5-inch LCD display with capacitive touch panel  
- **Interfaces:** CAN Bus, RS485, SD card, USB Type-C, RTC  
- **Power:** 5V via USB Type-C or 6–36V DC input, built-in battery charging circuit  

---

## 2. Pin Diagram

### ESP32-S3-WROOM-1 Pinout (Module U9)

| Pin No | GPIO | Function     | Description               |
|--------|------|--------------|---------------------------|
| 1      | GND  | Ground       | System ground             |
| 2      | 3V3  | Power        | 3.3V output               |
| 3      | EN   | Enable       | Module enable/disable     |
| 4      | IO4  | LCD_RST      | LCD reset line            |
| 5      | IO5  | CTP_RST      | Touch panel reset         |
| 6      | IO6  | LCD_DE       | Data Enable               |
| 7      | IO7  | PCLK         | LCD pixel clock           |
| 8      | IO15 | RS485_TXD    | RS485 transmit            |
| 9      | IO16 | RS485_RXD    | RS485 receive             |
| 10     | IO17 | CAN_TX       | CAN transmit              |
| 11     | IO18 | CAN_RX       | CAN receive               |
| 12     | IO19 | RTC_SDA      | RTC data line             |
| 13     | IO20 | RTC_SCL      | RTC clock line            |
| 14     | IO21 | LCD_G7       | LCD green data line       |
| 15     | IO38 | LCD_B4       | LCD blue data line        |
| 16     | IO39 | LCD_G2       | LCD green data line       |
| 17     | IO40 | LCD_R7       | LCD red data line         |
| 18     | IO41 | LCD_R6       | LCD red data line         |
| 19     | IO42 | LCD_R5       | LCD red data line         |
| 20     | IO45 | LCD_G4       | LCD green data line       |
| 21     | IO46 | HSYNC        | LCD horizontal sync       |
| 22     | IO47 | LCD_G5       | LCD green data line       |
| 23     | IO48 | VSYNC        | LCD vertical sync         |
| 24     | IO43 | RS485_RXD    | RS485 receive line        |
| 25     | IO44 | RS485_TXD    | RS485 transmit line       |
| 26     | IO0  | Boot         | Strapping pin             |
| 27     | IO1  | General I/O  | User-defined              |
| 28     | IO2  | General I/O  | User-defined              |
| 29     | IO3  | LCD_VSYNC    | LCD synchronization       |
| 30     | RXD0 | UART RX      | Serial communication      |
| 31     | TXD0 | UART TX      | Serial communication      |

---

## 3. Power Management

- **DC-DC Regulator:** SY8293FCC → 5V generation  
- **LDO Regulator:** SGM2212 → 3.3V generation  
- **Battery Charger:** CS8501 → Li-ion charging control  

---

## 4. Interfaces

- **LCD:** RGB data lines + HSYNC, VSYNC, PCLK, DE  
- **CTP (Touch Panel):** I²C (SDA, SCL) + IRQ, RST  
- **CAN Bus:** TJA1051T transceiver, CANH/CANL lines  
- **RS485:** SP3485EN transceiver, A/B differential lines  
- **SD Card:** SPI (MOSI, MISO, SCK, CS)  
- **USB Type-C:** DP, DN, CC1, CC2  

---

## 5. Protection

- **ESD Diodes:** LESD8LH5.0CT5G (USB, SD, CAN lines)  
- **Fuses:** BSMD1206-020/30 (USB and power lines)  

---

## 6. Application Scenarios

- Industrial control panels  
- Autonomous maritime/motor control systems (CAN/RS485)  
- IoT devices with touch interface  
- Data logging and visualization (SD card + LCD)  

---

## 7. PlatformIO + Arduino Development Guide

### Environment Setup

- Install **VSCode + PlatformIO extension**.  
- Add `esp32` platform in `platformio.ini`:  

  ```ini
  [env:esp32-s3-touch-lcd-5]
  platform = espressif32
  board = esp32-s3-devkitc-1
  framework = arduino
  monitor_speed = 115200
  ```

### Libraries

```ini
lib_deps =
    ESP32_Display_Panel@^0.1.4
    ESP32_IO_Expander@^0.0.4
    lvgl@^8.4.0
```

- **ESP32_Display_Panel** → LCD control  
- **ESP32_IO_Expander** → CH422G IO expander  
- **LVGL** → GUI framework  
- **lv_conf.h** → Must be added manually for LVGL configuration  

### Hardware Precautions

- Keep PCB antenna clear of metal/plastic.  
- TF card uses SPI, CS pin driven by CH422G EXIO4.  
- CAN/RS485 termination resistor optional.  
- LCD occupies most GPIO, IO expansion via CH422G.  
- Boot mode: hold BOOT button if USB not recognized.  

---

## 8. Demo Projects (Arduino Framework)

- **01_I2C_Test** → Scan devices on I²C bus  
- **02_RS485_Test** → Simple RS485 echo  
- **03_SD_Test** → TF card mount, file ops  
- **04_RTC_Test** → PCF85063A RTC + alarm  
- **05_IO_Test** → CH422G IO expander test  
- **06_TWAItransmit** → CAN bus transmit  
- **07_TWAIreceive** → CAN bus receive  
- **08_DrawColorBar** → RGB LCD color bar test  
- **09_lvgl_Porting** → LVGL GUI demo  

---

## 9. Example: LVGL Porting (PlatformIO)

```cpp
#include <lvgl.h>
#include <ESP32_Display_Panel.h>
#include <ESP32_IO_Expander.h>

void setup() {
  Serial.begin(115200);

  // Init IO Expander
  ESP_IOExpander_CH422G ioExpander;
  ioExpander.begin();

  // Init LCD
  ESP_PanelLcd lcd;
  lcd.begin();

  // Init LVGL
  lv_init();
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello ESP32-S3 LCD!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler(); // LVGL task handler
  delay(5);
}
```

---

## 10. Common Issues & Fixes

- **No LCD output after flashing:**  
  - Check Flash size (16MB), PSRAM enabled (8MB OPI).  
- **Missing `lv_conf.h`:**  
  - Ensure library path has no Chinese characters.  
- **Slow first compile:**  
  - Normal in PlatformIO, subsequent builds are faster.  
- **Red screen on IO test:**  
  - Insufficient USB power, use 5V/1A supply.  

---

## 11. Recommended Workflow

1. Start with **Arduino framework in PlatformIO** for quick prototyping.  
2. Use **LVGL demos** to validate LCD + touch.  
3. Integrate **CAN/RS485** demos for motor control evaluation.  
4. Document pinouts and demo results for investor-ready reports.
