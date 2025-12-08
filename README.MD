# 🍺 VibeGo - AlkoMetric Kiosk

**Eğlence Amaçlı Akıllı Alkolmetre Kiosk Sistemi**

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Display](https://img.shields.io/badge/Display-JC2432W328-green)
![Framework](https://img.shields.io/badge/Framework-Arduino%20%2B%20LVGL-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 📋 Proje Özeti

VibeGo, bar ve eğlence mekanları için tasarlanmış, modern dokunmatik ekranlı bir alkolmetre kiosk sistemidir. Kullanıcılar nefes üfleyerek kandaki tahmini alkol oranını (BAC) öğrenebilir.

> ⚠️ **Yasal Uyarı:** Bu cihaz yalnızca eğlence amaçlıdır. Sonuçların tıbbi veya yasal geçerliliği yoktur. Alkol aldıysanız kesinlikle araç kullanmayınız!

---

## 🖥️ Donanım Platformu

### Ana Kart: JC2432W328

| Özellik | Değer |
|---------|-------|
| **İşlemci** | ESP32-WROOM-32 (Dual-core, 240MHz) |
| **Ekran** | 2.8" TFT LCD, 320x240 (Landscape) |
| **Display Controller** | ST7789 (SPI) |
| **Touch Controller** | CST820 Capacitive (I2C) |
| **Bellek** | 520KB SRAM, 4MB Flash |
| **Bağlantı** | WiFi + Bluetooth LE |

### Pin Bağlantıları

**Display (SPI):**
| Pin | GPIO |
|-----|------|
| MOSI | 13 |
| SCLK | 14 |
| CS | 15 |
| DC | 2 |
| BL | 27 |

**Touch (I2C):**
| Pin | GPIO |
|-----|------|
| SDA | 33 |
| SCL | 32 |
| RST | 25 |
| INT | 21 |

---

## 🎨 Kullanıcı Arayüzü

### Ekran Akışı

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  DISCLAIMER │ ──► │    HOME     │ ──► │  MEASURING  │ ──► │   RESULT    │
│  Yasal Uyarı│     │  Ana Menü   │     │   Ölçüm     │     │   Sonuç     │
│             │     │             │     │             │     │             │
│  [KABUL ET] │     │ [TESTE BAŞ] │     │  (Spinner)  │     │  ✓ 0.15‰   │
└─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘
                                                                   │
                                                                   ▼
                                                            [YENİ TEST]
```

### Tema: Dark Mode

| Renk | Hex | Kullanım |
|------|-----|----------|
| Background | `#0F172A` | Ana arka plan |
| Panel | `#1E293B` | Kartlar |
| Primary | `#3B82F6` | Butonlar |
| Cyan | `#06B6D4` | Vurgular |
| Green | `#22C55E` | Güvenli sonuç |
| Red | `#EF4444` | Tehlikeli sonuç |

---

## � Kurulum

### Gereksinimler

- [PlatformIO](https://platformio.org/) (VS Code extension)
- USB-C kablo
- JC2432W328 geliştirme kartı

### Derleme ve Yükleme

```bash
# Projeyi derle
pio run

# Firmware'i yükle (COM portunu ayarlayın)
pio run -t upload --upload-port COM14

# Seri monitör
pio device monitor --port COM14 --baud 115200
```

---

## 📁 Proje Yapısı

```
VibeGo/
├── platformio.ini          # PlatformIO konfigürasyonu
├── src/
│   ├── main.cpp           # Ana program + sensör simülasyonu
│   ├── LGFX_Setup.h       # Display/Touch sürücü konfigürasyonu
│   ├── lv_conf.h          # LVGL konfigürasyonu
│   └── ui/
│       ├── ui.h           # UI tanımları
│       └── ui.c           # UI implementasyonu (4 ekran)
├── SquareLineProject/      # SquareLine Studio proje dosyaları
├── esp_32_s_3_touch_lcd.md # Donanım dokümantasyonu
└── README.md               # Bu dosya
```

---

## 🧪 Sensör Simülasyonu

Şu anda gerçek sensör bağlantısı olmadığından **simülasyon modu** aktiftir:

- Ölçüm süresi: **4 saniye**
- Sonuç dağılımı:
  - %70 ihtimalle **güvenli** (0.00 - 0.49‰)
  - %30 ihtimalle **tehlikeli** (0.50 - 2.00‰)

Simülasyonu devre dışı bırakmak için `main.cpp` içinde:
```cpp
#define SENSOR_SIMULATION_ENABLED false
```

---

## 📦 Kütüphaneler

| Kütüphane | Versiyon | Kullanım |
|-----------|----------|----------|
| [LovyanGFX](https://github.com/lovyan03/LovyanGFX) | ^1.1.16 | Display sürücüsü |
| [LVGL](https://lvgl.io/) | ^8.3.11 | UI framework |
| [ArduinoJson](https://arduinojson.org/) | ^6.21.3 | JSON işleme |

---

## � Gelecek Geliştirmeler

- [ ] ZE30 Elektrokimyasal alkol sensörü entegrasyonu
- [ ] SHT30 Nem sensörü ile nefes algılama
- [ ] Fan kontrolü (tahliye sistemi)
- [ ] WiFi üzerinden OTA güncelleme
- [ ] Uber/Taksi QR kodu entegrasyonu
- [ ] Çoklu dil desteği

---

## 📝 Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

## 👨‍� Geliştirici

**GoLabs Technology**

---

*Son Güncelleme: 08.12.2024*