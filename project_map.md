# 🗺️ VibeGo - Proje Haritası

**Son Güncelleme:** 09.12.2024  
**Sürüm:** v1.1.0  
**Durum:** Geliştirme Aşaması (Simülasyon Modu)  
**Platform:** JC2432W328 (ESP32-WROOM-32)

---

## 1. Mevcut Durum

### ✅ Tamamlanan Özellikler

| Özellik | Durum | Açıklama |
|---------|-------|----------|
| Display Sürücüsü | ✅ | ST7789 SPI, 320x240 landscape |
| Touch Sürücüsü | ✅ | CST820 capacitive I2C |
| LVGL Entegrasyonu | ✅ | v8.3 dark theme |
| UI Ekranları | ✅ | 4 ekran (Disclaimer, Home, Measuring, Result) |
| Sensör Simülasyonu | ✅ | 4sn ölçüm, rastgele sonuç |
| Dark Theme | ✅ | Modern lacivert/mavi tema |
| Geçiş Animasyonları | ✅ | Fade efektleri |
| WiFi Yönetimi | ✅ | WiFiManager Captive Portal |
| Otomatik Parlaklık | ✅ | LDR sensör (GPIO34) |
| Web Yönetim Paneli | ✅ | Responsive dashboard |
| SPIFFS Veri Loglama | ✅ | Son 100 test kaydı |
| NTP Zaman Sync | ✅ | UTC+3 Türkiye |
| HTTP OTA | ✅ | GitHub firmware güncellemesi |
| Watchdog Timer | ✅ | 30sn timeout |
| Cloud Sync | ✅ | Make.com → Google Sheets |

### 🔄 Devam Eden

| Özellik | Durum | Açıklama |
|---------|-------|----------|
| Gerçek Sensör | 🔄 | ZE30 + SHT30 entegrasyonu bekliyor |
| Fan Kontrolü | 🔄 | PWM sürücü hazır değil |

### ❌ Bekleyen

| Özellik | Öncelik | Açıklama |
|---------|---------|----------|
| ZE30 Alkol Sensörü | Yüksek | Gerçek ölçüm entegrasyonu |
| SHT30 Nem Sensörü | Yüksek | Ortam nem/sıcaklık |
| Fan PWM Kontrolü | Orta | Sensör havalandırma |
| Boot Splash | Düşük | Açılış animasyonu |
| QR Kod | Düşük | Uber/Taksi yönlendirme |
| Çoklu Dil | Düşük | İngilizce/Türkçe |

---

## 2. Dosya Yapısı

```
VibeGo/
│
├── 📄 platformio.ini         [Derleme konfigürasyonu - huge_app partition]
├── 📄 partitions_huge_app.csv [3MB uygulama alanı]
│
├── 📁 src/
│   ├── 📄 main.cpp           [Ana program + simülasyon + serial komutlar]
│   ├── 📄 LGFX_Setup.h       [ST7789 + CST820 sürücü]
│   ├── 📄 lv_conf.h          [LVGL ayarları]
│   │
│   ├── 📄 wifi_handler.h     [WiFiManager entegrasyonu]
│   ├── 📄 brightness.h       [LDR otomatik parlaklık]
│   ├── 📄 ntp_time.h         [NTP zaman senkronizasyonu]
│   ├── 📄 data_logger.h      [SPIFFS veri loglama]
│   ├── 📄 web_server.h       [Web yönetim paneli]
│   ├── 📄 webhook_logger.h   [Make.com cloud sync]
│   ├── 📄 http_ota.h         [GitHub OTA güncelleme]
│   │
│   └── 📁 ui/
│       ├── 📄 ui.h           [Ekran tanımları]
│       └── 📄 ui.c           [UI implementasyonu]
│
├── 📁 data/                   [SPIFFS dosyaları - web assets]
│
├── 📁 SquareLineProject/     [SquareLine Studio dosyaları]
│
├── 📄 README.MD              [Proje açıklaması]
├── 📄 CHANGELOG.md           [Sürüm geçmişi]
├── 📄 project_map.md         [Bu dosya]
└── 📄 esp_32_s_3_touch_lcd.md [Donanım referansı]
```

---

## 3. Sistem Mimarisi

```
┌─────────────────────────────────────────────────────────────────────┐
│                         VibeGo v1.1.0                               │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐          │
│  │   DISPLAY    │    │    TOUCH     │    │     LDR      │          │
│  │   ST7789     │    │   CST820     │    │   GPIO34     │          │
│  │   320x240    │    │     I2C      │    │  Parlaklık   │          │
│  └──────┬───────┘    └──────┬───────┘    └──────┬───────┘          │
│         │                   │                   │                   │
│         └───────────────────┴───────────────────┘                   │
│                             │                                       │
│                    ┌────────▼────────┐                              │
│                    │     LVGL 8.3    │                              │
│                    │   Dark Theme    │                              │
│                    └────────┬────────┘                              │
│                             │                                       │
│         ┌───────────────────┼───────────────────┐                   │
│         │                   │                   │                   │
│  ┌──────▼──────┐    ┌───────▼───────┐   ┌──────▼──────┐            │
│  │  UI Screens │    │  Web Server   │   │   WiFi      │            │
│  │  4 Ekran    │    │  Dashboard    │   │  Manager    │            │
│  └─────────────┘    └───────┬───────┘   └──────┬──────┘            │
│                             │                   │                   │
│         ┌───────────────────┼───────────────────┘                   │
│         │                   │                                       │
│  ┌──────▼──────┐    ┌───────▼───────┐                              │
│  │   SPIFFS    │    │   Webhook     │                              │
│  │ Data Logger │    │  Make.com     │                              │
│  │  100 kayıt  │    │ Google Sheets │                              │
│  └─────────────┘    └───────────────┘                              │
│                                                                     │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐          │
│  │   HTTP OTA   │    │   Watchdog   │    │     NTP      │          │
│  │   GitHub     │    │    30sn      │    │    UTC+3     │          │
│  └──────────────┘    └──────────────┘    └──────────────┘          │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 4. UI Akış Diyagramı

```
                    ┌──────────────────┐
                    │   Sistem Açılış  │
                    │  WiFi Bağlantısı │
                    │  NTP Sync        │
                    └────────┬─────────┘
                             │
                             ▼
              ┌──────────────────────────────┐
              │      DISCLAIMER EKRANI       │
              │  ⚠️ Yasal Uyarı Gösterimi     │
              │                              │
              │    [KABUL EDİYORUM] butonu   │
              └──────────────┬───────────────┘
                             │ (Tıklama)
                             ▼
              ┌──────────────────────────────┐
              │        HOME EKRANI           │
              │     VIBEGO Logo              │
              │  "Check Your Vibe"           │
              │                              │
              │    [▶ TESTE BAŞLA] butonu    │
              │                              │
              │    Kullanım Talimatları      │
              └──────────────┬───────────────┘
                             │ (Tıklama)
                             ▼
              ┌──────────────────────────────┐
              │      MEASURING EKRANI        │
              │                              │
              │    🔄 Spinner Animasyonu     │
              │   "Ölçüm Yapılıyor..."       │
              │   "Lütfen Üfleyin"           │
              │                              │
              │   ████████░░░░░ Progress     │
              │                              │
              │   (4 saniye simülasyon)      │
              └──────────────┬───────────────┘
                             │ (Otomatik)
              ┌──────────────┴───────────────┐
              │                              │
              ▼                              ▼
┌────────────────────┐          ┌────────────────────┐
│   RESULT: SAFE     │          │  RESULT: DANGER    │
│   ✓ GÜVENLİ        │          │  ✗ TEHLİKE         │
│                    │          │                    │
│      0.23          │          │      1.45          │
│     promil         │          │     promil         │
│                    │          │                    │
│  "İyi yolculuklar" │          │ "ARAÇ KULLANMA!"   │
│                    │          │                    │
│  [YENİ TEST]       │          │  [YENİ TEST]       │
└────────┬───────────┘          └────────┬───────────┘
         │                               │
         │   📊 SPIFFS'e kaydet          │
         │   ☁️ Cloud'a gönder           │
         │                               │
         └───────────────┬───────────────┘
                         │ (Tıklama)
                         ▼
                   DISCLAIMER'a dön
```

---

## 5. Web Panel Yapısı

```
http://[DEVICE_IP]/
│
├── 📊 Dashboard (Ana Sayfa)
│   ├── Günlük İstatistikler
│   │   ├── Toplam Test
│   │   ├── Güvenli Sayısı
│   │   └── Tehlikeli Sayısı
│   │
│   ├── Son Testler (20 kayıt)
│   │   ├── ID, Tarih, BAC, Sonuç
│   │   └── Tablo görünümü
│   │
│   └── Cihaz Bilgisi
│       ├── IP Adresi
│       ├── Firmware Sürümü
│       ├── Uptime
│       ├── WiFi Sinyal Gücü
│       └── LDR Değeri
│
├── ⚙️ Ayarlar
│   ├── Parlaklık Kontrolü (Slider)
│   ├── Otomatik Parlaklık (Toggle)
│   ├── Cloud Sync Süresi (Dropdown)
│   │   ├── Hemen
│   │   ├── 1 dakika
│   │   ├── 10 dakika
│   │   ├── 30 dakika
│   │   ├── 1 saat
│   │   └── 1 gün (varsayılan)
│   │
│   └── Aksiyonlar
│       ├── [Güncelleme Kontrol]
│       ├── [Logları Sil]
│       ├── [Şimdi Sync]
│       └── [Yeniden Başlat]
│
└── 📡 API Endpoints
    ├── GET  /api/stats     → İstatistikler
    ├── GET  /api/tests     → Test listesi
    ├── GET  /api/device    → Cihaz bilgisi
    ├── POST /api/settings  → Ayar güncelle
    ├── POST /api/sync      → Manuel sync
    └── POST /api/reboot    → Yeniden başlat
```

---

## 6. Renk Paleti

| Değişken | Hex | RGB | Kullanım |
|----------|-----|-----|----------|
| `UI_COLOR_BG_DARK` | #0F172A | 15,23,42 | Ana arka plan |
| `UI_COLOR_PANEL` | #1E293B | 30,41,59 | Panel arka planı |
| `UI_COLOR_PRIMARY` | #3B82F6 | 59,130,246 | Butonlar |
| `UI_COLOR_CYAN` | #06B6D4 | 6,182,212 | Vurgular |
| `UI_COLOR_GREEN` | #22C55E | 34,197,94 | Güvenli |
| `UI_COLOR_RED` | #EF4444 | 239,68,68 | Tehlikeli |
| `UI_COLOR_ORANGE` | #F59E0B | 245,158,11 | Uyarı |
| `UI_COLOR_TEXT_PRIMARY` | #FFFFFF | 255,255,255 | Ana metin |
| `UI_COLOR_TEXT_MUTED` | #94A3B8 | 148,163,184 | Soluk metin |
| `UI_COLOR_TEXT_BODY` | #CBD5E1 | 203,213,225 | Gövde metin |

---

## 7. Serial Komutları

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

## 8. Simülasyon Parametreleri

```cpp
// main.cpp içinde
#define SENSOR_SIMULATION_ENABLED true
#define SIMULATION_DURATION_MS 4000    // 4 saniye
#define BAC_THRESHOLD 0.50f            // Eşik değer

// Sonuç dağılımı:
// - %70: Güvenli (0.00 - 0.49‰)
// - %30: Tehlikeli (0.50 - 2.00‰)
```

---

## 9. Bellek Yapılandırması

```
Partition Table: huge_app.csv
┌──────────────┬────────┬──────────┐
│ Partition    │ Type   │ Size     │
├──────────────┼────────┼──────────┤
│ nvs          │ data   │ 20KB     │
│ otadata      │ data   │ 8KB      │
│ app0         │ app    │ 3MB      │
│ spiffs       │ data   │ 896KB    │
└──────────────┴────────┴──────────┘

LVGL: Çift tamponlama (45KB x 2)
Stack: 16KB
```

---

## 10. Derleme Komutları

```bash
# Temiz derleme
pio run --target clean && pio run

# Yükleme
pio run -t upload --upload-port COM14

# SPIFFS yükleme
pio run -t uploadfs --upload-port COM14

# Seri monitör
pio device monitor --port COM14 --baud 115200

# Bellek kullanımı
pio run -t size
```

---

## 11. Sonraki Adımlar

### Kısa Vadeli (Bu Hafta)

1. [ ] ZE30 sensör entegrasyonu
2. [ ] SHT30 nem sensörü
3. [ ] Fan PWM kontrolü

### Orta Vadeli (Bu Ay)

1. [ ] Boot splash animasyonu
2. [ ] Sensör kalibrasyon ekranı
3. [ ] Gelişmiş istatistikler

### Uzun Vadeli

1. [ ] QR kod gösterimi
2. [ ] Çoklu dil desteği
3. [ ] Bluetooth entegrasyonu
4. [ ] Mobil uygulama

---

## 12. Sürüm Geçmişi

| Sürüm | Tarih | Öne Çıkanlar |
|-------|-------|--------------|
| v1.1.0 | 09.12.2024 | Web Panel, Cloud Sync, HTTP OTA, NTP, Watchdog |
| v1.0.0 | 08.12.2024 | İlk sürüm - LVGL UI, WiFi, LDR |

---

*Proje Haritası v1.1.0*
