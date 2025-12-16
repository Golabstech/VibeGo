# VibeGo - Kapsamlı Sistem Analizi ve Düzeltme Planı

## 🔍 Tespit Edilen Sorunlar

### 1. **Result Ekranı Reset Sorunu**
- ✅ UI optimizasyonu yapıldı (QR 250px→180px)
- ✅ Bellek temizliği eklendi
- ⚠️ Sorun devam ediyor

### 2. **Web Dashboard Çalışmıyor**
- Web server başlatılıyor mu?
- API endpoint'leri çalışıyor mu?
- WiFi bağlantısı var mı?

### 3. **Olası Kök Nedenler**
1. **Bellek Yetersizliği**: ESP32-S3'te PSRAM kullanımı yanlış yapılandırılmış olabilir
2. **LVGL Buffer Sorunu**: 20 satırlık buffer çok küçük olabilir
3. **Watchdog Timeout**: 60sn timeout çok uzun, sistem donuyor olabilir
4. **Task Çakışması**: LVGL + WiFi + Web Server aynı anda çalışırken çakışma
5. **Stack Overflow**: Arduino loop'ta çok fazla işlem

## 🎯 Çözüm Stratejisi

### Aşama 1: Temel Sorunları Düzelt
1. ✅ LVGL buffer boyutunu artır (20→40 satır)
2. ✅ Watchdog timeout'u azalt (60s→30s)
3. ✅ Result ekranını daha da basitleştir
4. ✅ Web server'ı ayrı task'ta çalıştır

### Aşama 2: Bellek Yönetimini İyileştir
1. ✅ PSRAM kullanımını optimize et
2. ✅ Heap monitoring ekle
3. ✅ Memory leak kontrolü

### Aşama 3: Test ve Doğrulama
1. Serial monitor ile debug
2. Heap kullanımını izle
3. Crash log analizi

## 📋 Yapılacaklar Listesi

### Kritik (Hemen)
- [ ] LVGL buffer boyutunu 40 satıra çıkar
- [ ] Watchdog 30s yap
- [ ] Result ekranını daha da basitleştir (QR'ı kaldır geçici olarak)
- [ ] Web server'ı test et
- [ ] Serial debug ekle

### Orta Öncelik
- [ ] PSRAM allocation'ı kontrol et
- [ ] Heap monitoring sistemi ekle
- [ ] WiFi reconnect mekanizması düzelt

### Düşük Öncelik
- [ ] OTA update test
- [ ] NTP sync optimize et
- [ ] Webhook queue optimize et

## 🔧 Önerilen Değişiklikler

### 1. main.cpp
```cpp
// Buffer boyutunu artır
int buffer_lines = 40; // 20→40

// Watchdog timeout'u azalt
esp_task_wdt_init(30, true); // 60s→30s, panic enabled

// Heap monitoring ekle
if (esp_get_free_heap_size() < 50000) {
    Serial.println("[WARN] Low heap!");
}
```

### 2. ui.c - Result Ekranını Basitleştir
```c
// GEÇİCİ: QR kod'u kaldır
#if 0  // QR disabled temporarily
    ui_Result_QR = lv_qrcode_create(...);
#endif
```

### 3. Web Server - Ayrı Task
```cpp
// Web server'ı ayrı task'ta çalıştır
xTaskCreatePinnedToCore(
    web_server_task,
    "WebServer",
    8192,
    NULL,
    1,
    NULL,
    0  // Core 0
);
```

## 🚨 Acil Eylem Planı

1. **İlk Adım**: Serial monitor'dan crash log al
2. **İkinci Adım**: QR kod'u geçici olarak kaldır
3. **Üçüncü Adım**: Buffer boyutunu artır
4. **Dördüncü Adım**: Web dashboard'u test et
5. **Beşinci Adım**: Sorun devam ederse sıfırdan minimal sistem yaz

## 📊 Beklenen Sonuçlar

### Başarı Kriterleri
- ✅ Cihaz reset atmıyor
- ✅ Tüm ekran geçişleri sorunsuz
- ✅ Web dashboard erişilebilir
- ✅ Bellek kullanımı stabil (>50KB free heap)
- ✅ Uptime >1 saat

### Performans Metrikleri
- Free Heap: >50KB (şu an: ?)
- Loop Duration: <50ms
- LVGL Handler: <30ms
- WiFi Reconnect: <10s

---

**Sonraki Adım**: Serial monitor çıktısını görmek ve crash nedenini tespit etmek.
