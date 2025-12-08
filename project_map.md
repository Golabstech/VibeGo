# 🗺️ VibeGo - Proje Haritası

**Son Güncelleme:** 08.12.2024  
**Durum:** Geliştirme Aşaması (Simülasyon Modu)  
**Platform:** JC2432W328

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

### 🔄 Devam Eden

| Özellik | Durum | Açıklama |
|---------|-------|----------|
| Gerçek Sensör | 🔄 | ZE30 + SHT30 entegrasyonu bekliyor |
| Fan Kontrolü | 🔄 | PWM sürücü hazır değil |

### ❌ Bekleyen

| Özellik | Öncelik | Açıklama |
|---------|---------|----------|
| QR Kod | Orta | Uber/Taksi yönlendirme |
| WiFi OTA | Düşük | Kablosuz güncelleme |
| Veri Loglama | Düşük | SD kart desteği |

---

## 2. Dosya Yapısı

```
VibeGo/
│
├── 📄 platformio.ini         [Derleme konfigürasyonu]
│
├── 📁 src/
│   ├── 📄 main.cpp           [Ana program + simülasyon]
│   ├── 📄 LGFX_Setup.h       [ST7789 + CST820 sürücü]
│   ├── 📄 lv_conf.h          [LVGL ayarları]
│   │
│   └── 📁 ui/
│       ├── 📄 ui.h           [Ekran tanımları]
│       └── 📄 ui.c           [UI implementasyonu]
│
├── 📁 SquareLineProject/     [SquareLine Studio dosyaları]
│
├── 📄 README.MD              [Proje açıklaması]
├── 📄 project_map.md         [Bu dosya]
└── 📄 esp_32_s_3_touch_lcd.md [Donanım referansı]
```

---

## 3. UI Akış Diyagramı

```
                    ┌──────────────────┐
                    │   Sistem Açılış  │
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
         └───────────────┬───────────────┘
                         │ (Tıklama)
                         ▼
                   DISCLAIMER'a dön
```

---

## 4. Renk Paleti

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

## 5. Simülasyon Parametreleri

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

## 6. Derleme Komutları

```bash
# Temiz derleme
pio run --target clean && pio run

# Yükleme
pio run -t upload --upload-port COM14

# Seri monitör
pio device monitor --port COM14 --baud 115200

# Bellek kullanımı
pio run -t size
```

---

## 7. Sonraki Adımlar

### Kısa Vadeli (Bu Hafta)
1. [ ] Touch kalibrasyonu kontrol
2. [ ] Ekran parlaklık ayarı
3. [ ] Animasyon iyileştirmeleri

### Orta Vadeli (Bu Ay)
1. [ ] ZE30 sensör entegrasyonu
2. [ ] SHT30 nem sensörü
3. [ ] Fan PWM kontrolü

### Uzun Vadeli
1. [ ] WiFi bağlantısı
2. [ ] OTA güncelleme
3. [ ] QR kod gösterimi
4. [ ] Çoklu dil desteği

---

*Proje Haritası v1.0*
