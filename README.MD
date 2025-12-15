# 🍺 VibeGo - AlkoMetric Kiosk

**Akıllı Alkolmetre Kiosk Sistemi**

![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Display](https://img.shields.io/badge/Display-Waveshare%205inch-green)
![Framework](https://img.shields.io/badge/Framework-Arduino%20%2B%20LVGL-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-purple)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 📋 Proje Özeti

VibeGo, bar ve mekanlar için tasarlanmış, modern dokunmatik ekranlı bir alkolmetre kiosk sistemidir. Kullanıcılar nefes üfleyerek kandaki tahmini alkol oranını (BAC) öğrenebilir.

> ⚠️ **DİKKAT:** Bu cihaz yasal veya tıbbi ölçüm yapmaz. Sonuçlar tahminidir. Alkol aldıysanız **KESİNLİKLE ARAÇ KULLANMAYIN!**

---

## ✨ Güncel Özellikler (v2.0.0)

### 🖥️ Yeni Platform: Waveshare ESP32-S3-Touch-LCD-5

- 5 inç 1024x600 IPS LCD ekran
- ESP32-S3 (16MB Flash, 8MB PSRAM)
- GT911 kapasitif dokunmatik panel
- RGB565 paralel arayüz

### 🎬 Boot Splash Ekranı

- Golabs logosu ile profesyonel açılış ekranı
- Animasyonlu progress bar
- Versiyon gösterimi

### 🔲 QR Kod & Sponsor Sistemi

- Result ekranında dinamik QR kod gösterimi
- QR tarama ile sponsor URL'ine yönlendirme (Uber, BiTaksi, Bolt vb.)
- Tarama istatistikleri ve sayaç
- Web panelden sponsor ayarları yönetimi

### 🌐 Web Yönetim Paneli

- **4 Sekmeli Dashboard:** Anasayfa, WiFi, Ayarlar, Sponsor
- Günlük istatistikler ve son test kayıtları
- Cihaz bilgisi (IP, firmware, uptime, sinyal gücü)
- Parlaklık kontrolü (manuel/otomatik LDR)

### 📶 WiFi AP+STA Modu

- Daima erişilebilir Access Point (`VibeGo` / `12345678`)
- Web üzerinden ağ tarama ve bağlantı
- Hibrit mod: AP ve Client aynı anda aktif

### ☁️ Cloud Entegrasyonu

- Make.com webhook ile Google Sheets entegrasyonu
- Offline kuyruk sistemi
- Ayarlanabilir sync süresi (hemen, dakika, saat, gün)

### ⏰ Zaman & Güvenlik

- NTP senkronizasyonu (Türkiye timezone)
- Watchdog Timer (30sn timeout)
- HTTP OTA otomatik güncelleme

---

## 🖥️ Donanım Platformu

### Ana Kart: Waveshare ESP32-S3-Touch-LCD-5

| Özellik | Değer |
|---------|-------|
| **İşlemci** | ESP32-S3-WROOM-1-N16R8 (Dual-core, 240MHz) |
| **Ekran** | 5" IPS LCD, 1024x600 (Landscape) |
| **Display Interface** | RGB565 (16-bit Parallel) |
| **Touch Controller** | GT911 Capacitive (I2C, 5-point) |
| **IO Expander** | CH422G (Backlight, Reset) |
| **Bellek** | 16MB Flash, 8MB PSRAM |
| **Bağlantı** | WiFi + Bluetooth 5 LE |
| **Arayüzler** | CAN, RS485, I2C, USB-C |

---

## 🎨 Kullanıcı Arayüzü

### Ekran Akışı

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  DISCLAIMER │ ──► │    HOME     │ ──► │  MEASURING  │ ──► │   RESULT    │
│  Yasal Uyarı│     │  Ana Menü   │     │   Ölçüm     │     │  Sonuç + QR │
│             │     │             │     │             │     │             │
│  [KABUL ET] │     │ [TESTE BAŞ] │     │  (Spinner)  │     │ [YENİ TEST] │
└─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘
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

## 🚀 Kurulum

### Gereksinimler

- [PlatformIO](https://platformio.org/) (VS Code extension)
- USB-C kablo
- Waveshare ESP32-S3-Touch-LCD-5 geliştirme kartı

### Derleme ve Yükleme

```bash
# Projeyi derle
pio run -e waveshare_5inch

# Firmware'i yükle
pio run -e waveshare_5inch -t upload

# Seri monitör
pio device monitor --baud 115200
```

---

## 📁 Proje Yapısı

```
VibeGo/
├── platformio.ini          # PlatformIO konfigürasyonu
├── CHANGELOG.md            # Versiyon geçmişi
├── src/
│   ├── main.cpp            # Ana program
│   ├── LGFX_Setup.h        # Display/Touch sürücüsü
│   ├── lv_conf.h           # LVGL konfigürasyonu
│   ├── wifi_handler.h      # WiFi AP+STA modülü
│   ├── http_ota.h          # HTTP OTA güncelleme
│   ├── brightness.h        # LDR parlaklık modülü
│   ├── ntp_time.h          # NTP zaman modülü
│   ├── data_logger.h       # SPIFFS veri loglama
│   ├── web_server.h        # Web yönetim paneli
│   ├── webhook_logger.h    # Make.com entegrasyonu
│   ├── sponsor_manager.h   # Sponsor/QR yönetimi
│   └── ui/
│       ├── ui.h            # UI tanımları
│       └── ui.c            # UI implementasyonu
├── SquareLineProject/       # SquareLine Studio dosyaları
└── firmware/                # Build çıktıları
```

---

## 🎯 Gelecek Geliştirmeler

Detaylı ticarileşme planı için: **[ROADMAP.md](ROADMAP.md)**

### Yakın Dönem (Q1 2025)

- [ ] ZE30 elektrokimyasal alkol sensörü
- [ ] SHT30 nem sensörü (nefes algılama)
- [ ] Fan PWM kontrolü
- [ ] Boot splash animasyonu

### Orta Dönem (Q2-Q3 2025)

- [ ] Reklam & Sponsor sistemi
- [ ] Çoklu dil desteği (TR/EN/DE/RU)
- [ ] Sesli geri bildirim
- [ ] Merkezi yönetim paneli

---

## 📦 Kütüphaneler

| Kütüphane | Versiyon | Kullanım |
|-----------|----------|----------|
| [LovyanGFX](https://github.com/lovyan03/LovyanGFX) | ^1.1.16 | Display sürücüsü |
| [LVGL](https://lvgl.io/) | ^8.3.11 | UI framework |
| [ArduinoJson](https://arduinojson.org/) | ^6.21.3 | JSON işleme |

---

## 🔧 Serial Komutları

| Komut | Açıklama |
|-------|----------|
| `status` | Sistem durumunu göster |
| `dashboard` | Web panel URL'sini göster |
| `wifi_portal` | WiFi ayar portalını başlat |
| `wifi_reset` | WiFi ayarlarını sıfırla |
| `check_update` | OTA güncelleme kontrolü |
| `reboot` | Cihazı yeniden başlat |
| `help` | Komut listesi |

---

## 📝 Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

---

## 👨‍💻 Geliştirici

**GoLabs Technology**

---

*Son Güncelleme: 15.12.2024 | v2.0.0*
