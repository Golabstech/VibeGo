# 📋 VibeGo - Değişiklik Günlüğü (Changelog)

## v1.3.0 (09.12.2024)

### 🔲 QR Kod Entegrasyonu

- **Result Ekranında QR Kod:** LVGL QR kod widget'ı ile dinamik QR gösterimi
- **Sponsor Yönlendirme:** QR tarandığında sponsor URL'ine yönlendirme
- **Tarama Sayacı:** `/go` endpoint'i ile tarama istatistikleri
- **Dinamik Güncelleme:** Web panelden değişiklikler anında cihaza yansır

### 🎯 Sponsor Yönetim Modülü

- **Yeni Dosya:** `sponsor_manager.h` - NVS'de kalıcı sponsor ayarları
- **Web Panel Sponsor Sekmesi:** 4. sekme olarak eklendi
- **Ayarlanabilir Alanlar:**
  - QR Yönlendirme URL
  - Sponsor Adı
  - Sponsor Telefon
  - QR Başlık Metni (cihaz ekranında görünen)
- **Hızlı Linkler:** Uber, BiTaksi, Bolt için hazır URL'ler
- **Tarama İstatistikleri:** Toplam tarama sayısı ve aktif sponsor gösterimi

### ⏰ NTP Senkronizasyon İyileştirmeleri

- **Türkiye NTP Sunucusu:** `tr.pool.ntp.org` birincil sunucu olarak eklendi
- **Timeout Artışı:** 5 saniyeden 15 saniyeye çıkarıldı
- **Retry Mekanizması:** Başarısız olursa 60 saniyede bir, max 5 kez retry
- **WiFi Reconnect:** WiFi yeniden bağlandığında NTP otomatik restart
- **Detaylı Loglama:** Retry sayısı ve başarı durumu serial'de gösterilir

### 📱 UI İyileştirmeleri

- **Measuring Ekranı:** Spinner sol, metin sağ - üst üste binme düzeltildi
- **Result Ekranı Yeniden Tasarlandı:**
  - Sol panel: BAC değeri ve durum
  - Sağ panel: QR kod ve yeni test butonu
- **QR Başlık:** İkon kaldırıldı, text wrap eklendi (uzun metinler alt satıra geçer)
- **Home Ekranı:** Kullanım talimatları güncellendi (5 adım)

### 🛠️ Teknik Değişiklikler

- **lv_conf.h Güncellemeleri:**
  - `LV_USE_QRCODE = 1` (QR widget etkin)
  - `LV_USE_CANVAS = 1` (QR için gerekli)
  - `LV_FONT_MONTSERRAT_10 = 1` (küçük font eklendi)
- **Yeni API Endpoints:**
  - `GET /api/sponsor` - Sponsor ayarlarını getir
  - `POST /api/sponsor` - Sponsor ayarlarını kaydet
  - `POST /api/sponsor/reset_scans` - Tarama sayacını sıfırla
  - `GET /go` - QR redirect (tarama logla + yönlendir)

---

## v1.2.0 (09.12.2024)

### 📶 WiFi AP+STA Modu

- **Daima Erişilebilir AP:** ESP32 her zaman `VibeGo` AP'si olarak yayın yapar ayarlanan sürede local ağa bağlanırsa (10dk) ap modu otomatik kapanır.
- **AP IP:** `192.168.4.1` - Her zaman bu adresten dashboard'a erişilebilir
- **Hibrit Mod:** Hem AP hem de STA (client) modu aynı anda aktif
- **Şifre:** `12345678` (WPA2)

### 🌐 Web Üzerinden WiFi Yönetimi

- **3 Sekmeli Dashboard:** Dashboard, WiFi, Ayarlar
- **Ağ Tarama:** Mevcut WiFi ağlarını listele
- **Bağlantı:** Seçilen ağa şifre ile bağlan
- **Durum Gösterimi:** Bağlı ağ, IP, sinyal gücü
- **Ayar Sıfırlama:** Kayıtlı ağ bilgilerini sil

### 🗑️ Kaldırılan

- **WiFiManager Kütüphanesi:** Artık kendi WiFi modülümüz kullanılıyor
- **Captive Portal:** Web dashboard ile değiştirildi

### 💾 Preferences API

- WiFi bilgileri NVS'de kalıcı olarak saklanır
- Yeniden başlatmada otomatik bağlanır

---

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
