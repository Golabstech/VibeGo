# 📋 VibeGo - Değişiklik Günlüğü (Changelog)

## v1.1.0 (09.12.2024)

### 🌐 Web Yönetim Paneli
- **Responsive Dashboard:** `http://[IP]/` adresinden erişilebilir
- **Günlük İstatistikler:** Toplam, güvenli, tehlikeli test sayıları
- **Son Testler:** Son 20 test kaydı
- **Cihaz Bilgisi:** IP, firmware, uptime, WiFi sinyal gücü, LDR

### ⚙️ Web Tabanlı Ayarlar
- **Parlaklık Kontrolü:** Manuel slider ile ayar
- **Otomatik Parlaklık:** LDR ile açık/kapalı
- **Cloud Sync Süresi:** Hemen, 1dk, 10dk, 30dk, 1 saat, 1 gün
- **Aksiyonlar:** Güncelleme kontrol, logları sil, yeniden başlat

### ☁️ Make.com Webhook Entegrasyonu
- **Google Sheets Entegrasyonu:** Test sonuçları otomatik olarak Google Sheets'e yazılır
- **Kuyruk Sistemi:** Offline durumlarda veriler kuyrukta saklanır
- **Ayarlanabilir Sync:** Varsayılan günlük, istenirse hemen/dakika/saat
- **Manuel Sync:** "Şimdi Sync" butonu ile anında gönderim

### 📊 SPIFFS Veri Loglama
- Test sonuçları cihazda saklanır (ID, tarih, BAC, sonuç)
- Son 100 test kaydı tutulur
- Günlük istatistikler hesaplanır

### ⏰ NTP Zaman Senkronizasyonu
- UTC+3 (Türkiye) zaman dilimi
- Otomatik senkronizasyon
- Uptime takibi

### 🛡️ Watchdog Timer
- 30 saniyelik timeout
- Sistem donmalarını önler
- Otomatik yeniden başlatma

### 🔄 HTTP OTA Güncelleme
- GitHub üzerinden firmware güncellemesi
- Saatlik otomatik kontrol
- Serial komutu: `check_update`

### ⚡ Bellek Optimizasyonları
- `huge_app.csv` partition tablosu (3MB uygulama alanı)
- LVGL çift tamponlama
- Stack boyutu 16KB'a artırıldı
- Gereksiz fontlar kaldırıldı

### 🗑️ Kaldırılan Özellikler
- Cihaz üstü Settings ekranı (web panel'e taşındı)
- Logo tap ile servis modu (artık sadece web panel)
- ArduinoOTA (HTTP OTA ile değiştirildi)

---

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

### 💡 Otomatik Parlaklık (LDR)
- GPIO34 LDR sensörü
- Ortam ışığına göre otomatik ekran parlaklığı

---

## Serial Komutları

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

## Dosya Yapısı

```
src/
├── main.cpp          # Ana program
├── LGFX_Setup.h      # Display/Touch sürücüsü
├── lv_conf.h         # LVGL konfigürasyonu
├── wifi_handler.h    # WiFi modülü
├── http_ota.h        # HTTP OTA modülü
├── brightness.h      # LDR parlaklık modülü
├── ntp_time.h        # NTP zaman modülü
├── data_logger.h     # SPIFFS veri loglama
├── web_server.h      # Web yönetim paneli
├── webhook_logger.h  # Make.com entegrasyonu
└── ui/
    ├── ui.h          # UI tanımları
    └── ui.c          # UI implementasyonu
```

---

## Sonraki Adımlar

- [ ] ZE30 alkol sensörü entegrasyonu
- [ ] SHT30 nem sensörü
- [ ] Fan PWM kontrolü
- [ ] Boot splash animasyonu
