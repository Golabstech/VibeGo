# JC2432W328 ESP32 Touch LCD - Donanım Dokümantasyonu

Bu doküman JC2432W328 geliştirme kartının pin yapısını ve yapılandırmasını içerir.

---

## 1. Kart Özellikleri

| Özellik | Değer |
|---------|-------|
| **Ana İşlemci** | ESP32-WROOM-32 (Dual-core, 240MHz) |
| **Bellek** | 520KB SRAM, 448KB ROM, 4MB Flash |
| **Bağlantı** | WiFi + Bluetooth LE |
| **Ekran** | 2.8" TFT LCD, 240x320 piksel, 65K renk |
| **Ekran Kontrolcüsü** | ST7789 (SPI arayüzü) |
| **Touch Kontrolcüsü** | CST820 (I2C, Capacitive) |
| **Ek Özellikler** | TF kart yuvası, hoparlör sürücüsü |
| **Çalışma Gerilimi** | 5V |

---

## 2. Pin Bağlantı Şeması

### 2.1 Ekran (ST7789 - SPI)

| Fonksiyon | GPIO | Açıklama |
|-----------|------|----------|
| TFT_MOSI | GPIO 13 | SPI veri çıkışı |
| TFT_SCLK | GPIO 14 | SPI saat sinyali |
| TFT_CS | GPIO 15 | Chip select |
| TFT_DC | GPIO 2 | Data/Command seçimi |
| TFT_RST | -1 | Reset (dahili) |
| TFT_BL | GPIO 27 | Backlight kontrolü (PWM) |

### 2.2 Dokunmatik Panel (CST820 - I2C)

| Fonksiyon | GPIO | Açıklama |
|-----------|------|----------|
| TOUCH_SDA | GPIO 33 | I2C veri hattı |
| TOUCH_SCL | GPIO 32 | I2C saat hattı |
| TOUCH_RST | GPIO 25 | Touch reset |
| TOUCH_INT | GPIO 21 | Touch interrupt |

### 2.3 SD Kart (SPI)

| Fonksiyon | GPIO |
|-----------|------|
| SD_MOSI | GPIO 23 |
| SD_MISO | GPIO 19 |
| SD_SCK | GPIO 18 |
| SD_CS | GPIO 5 |

---

## 3. Blok Diyagram

```
         +----------------------+
         |      USB Type-C      |
         +----------+-----------+
                    |
                 [5V]
                    |
         +----------v-----------+
         |  Power Management    |
         |    LDO 3.3V          |
         +----------+-----------+
                    |
                  [3V3]
                    |
         +----------v-----------+
         |   ESP32-WROOM-32     |
         |   WiFi / BLE / USB   |
         +--+---+---+----+---+--+
            |   |   |    |   |
            |   |   |    |   +---> SD Card (SPI)
            |   |   |    +-------> Speaker
            |   |   +------------> Touch Panel (I2C)
            +---+----------------> LCD ST7789 (SPI)
```

---

## 4. Yazılım Konfigürasyonu

### PlatformIO Ortamı

```ini
[env:jc2432w328]
platform = espressif32@6.5.0
board = esp32dev
framework = arduino
```

### LovyanGFX Ayarları

- **Panel:** `lgfx::Panel_ST7789`
- **Bus:** `lgfx::Bus_SPI` (VSPI_HOST)
- **Touch:** `lgfx::Touch_CST816S` (CST820 uyumlu)
- **SPI Hızı:** 40MHz write, 16MHz read
- **I2C Adresi:** 0x15 (Touch)

---

## 5. Sensör Bağlantısı (AlkoMetric Projesi)

### SHT30 Nem Sensörü (I2C)
> **Not:** Touch panel ile aynı I2C hattını kullanamaz! Ayrı I2C portu gerekir.

| Fonksiyon | GPIO | Açıklama |
|-----------|------|----------|
| SHT_SDA | GPIO 21 | I2C veri (Wire1) |
| SHT_SCL | GPIO 22 | I2C saat (Wire1) |

### ZE30 Alkol Sensörü (UART/RS485)
| Fonksiyon | GPIO |
|-----------|------|
| TX | GPIO 17 |
| RX | GPIO 16 |

### Fan Kontrolü
| Fonksiyon | GPIO |
|-----------|------|
| FAN_PWM | GPIO 4 |

---

## 6. Önemli Notlar

1. **Touch I2C Çakışması:** CST820 touch kontrolcüsü GPIO 32/33 kullanır. Ek I2C cihazları için `Wire1` kullanın.

2. **Backlight PWM:** GPIO 27 üzerinden parlaklık kontrolü yapılabilir (0-255).

3. **Flash Modu:** `dio` modunda derleme yapılmalıdır.

4. **PSRAM:** Bu kartta PSRAM yoktur. Bellek yönetimine dikkat edin.

---

*Son Güncelleme: 08.12.2024*
