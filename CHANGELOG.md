# 📋 VibeGo - Değişiklik Günlüğü (Changelog)

## v1.0.0 (08.12.2024)

### 🎯 İlk Sürüm - JC2432W328 Platform Desteği

#### Donanım
- **Display:** ST7789 2.8" TFT LCD (320x240 Landscape)
- **Touch:** CST820 Capacitive I2C  
- **Platform:** ESP32-WROOM-32

#### Temel Özellikler
- ✅ LVGL 8.3 Dark Theme UI
- ✅ 4 Ekran: Disclaimer → Home → Measuring → Result
- ✅ Sensör simülasyonu (4sn, rastgele BAC değeri)
- ✅ Animasyonlu ekran geçişleri

---

### 🌐 WiFi Entegrasyonu
- WiFiManager ile Captive Portal (`VibeGo-Setup`)
- Kayıtlı ağlara otomatik bağlanma (5sn timeout)
- Serial komut: `wifi_portal` - WiFi ayar portalını başlatır
- Serial komut: `wifi_reset` - Kayıtlı ağları siler

### 🔄 OTA Güncelleme
- ArduinoOTA desteği
- Hostname: `vibego.local:3232`
- PlatformIO ile OTA yükleme: `pio run -e ota -t upload`

### 💡 Otomatik Parlaklık (LDR)
- GPIO34 LDR sensörü
- Ortam ışığına göre otomatik ekran parlaklığı
- 5 saniyede bir debug çıktısı

### ⚙️ Servis Modu (Settings)
- **Erişim:** VIBEGO logosuna 5 kez tıkla (3sn içinde)
- **Alternatif:** Serial'den `settings` yaz
- **Gösterilen bilgiler:**
  - WiFi durumu ve IP adresi
  - Parlaklık seviyesi (Oto/Manuel)
  - Firmware versiyonu
  - OTA hostname

---

## Serial Komutları

| Komut | Açıklama |
|-------|----------|
| `settings` | Settings ekranını aç |
| `status` | Sistem durumunu göster |
| `wifi_portal` | WiFi ayar portalını başlat |
| `wifi_reset` | WiFi ayarlarını sıfırla |
| `reboot` | Cihazı yeniden başlat |
| `help` | Komut listesi |

---

## Dosya Yapısı

```
src/
├── main.cpp          # Ana program
├── LGFX_Setup.h      # Display/Touch sürücüsü
├── lv_conf.h         # LVGL konfigürasyonu (optimize)
├── wifi_handler.h    # WiFi modülü
├── ota_handler.h     # OTA modülü
├── brightness.h      # LDR parlaklık modülü
└── ui/
    ├── ui.h          # UI tanımları
    └── ui.c          # UI implementasyonu (5 ekran)
```

---

## Bellek Kullanımı

| Kaynak | Kullanım | Kapasite |
|--------|----------|----------|
| RAM | ~30% | 97KB / 327KB |
| Flash | ~99% | 1.29MB / 1.31MB |

---

## Bilinen Sorunlar

- Flash doluluk oranı yüksek (~99%)
- WiFi aktifken ADC2 kullanılamaz (GPIO0)

---

## Sonraki Adımlar

- [ ] ZE30 alkol sensörü entegrasyonu
- [ ] SHT30 nem sensörü
- [ ] Fan PWM kontrolü
- [ ] Boot splash animasyonu
