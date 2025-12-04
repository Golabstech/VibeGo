# 🍺 AlkoMetric Kiosk - Proje Geliştirme Dokümanı

> **Proje Tanımı:** ESP32-S3 tabanlı, dokunmatik ekranlı, modern arayüze sahip ve bulut entegrasyonlu (QR Taksi) ticari alkolmetre kiosk projesi.

---

## 📋 1. Donanım Listesi (BOM)

Bu proje **JC4827W543** geliştirme kartı üzerine kuruludur.

| Bileşen | Model / Özellik | Adet | Notlar |
| :--- | :--- | :--- | :--- |
| **Ana Kart + Ekran** | JC4827W543 (ESP32-S3) | 1 | 4.3 inç veya 5.0 inç Kapasitif Dokunmatik |
| **Alkol Sensörü** | MQ-3 Modülü | 1 | Isıtıcı devresi için 5V besleme şart |
| **ADC Dönüştürücü** | (Opsiyonel) ADS1115 | 1 | Daha hassas ölçüm için harici ADC önerilir |
| **Güç Kaynağı** | 5V 3A Adaptör | 1 | Sensör ısıtıcısı akım çeker, kaliteli adaptör şart |
| **Kasa** | Özel Tasarım | 1 | 3D Baskı veya Pleksi Lazer Kesim |
| **Soğutma** | Mini Fan (30mm) | 1 | Sensörün temiz hava alması (tahliye) için |

---

## ⚡ 2. Bağlantı Şeması (Wiring Scheme)

**ÖNEMLİ:** ESP32 girişleri **3.3V** ile çalışır. MQ-3 sensörü ise **5V** ile çalışır ve 0-5V arası analog çıktı verebilir. ESP32'yi yakmamak için bir **Voltaj Bölücü** (Voltage Divider) kullanmalısın.

### MQ-3 Sensör Bağlantısı



[Image of basic voltage divider circuit with 2 resistors]


1.  **VCC (Sensör):** -> Harici 5V Kaynağına (ESP32'nin 5V pininden çekme, harici besle)
2.  **GND (Sensör):** -> Ortak GND (ESP32 GND ile birleştir)
3.  **A0 (Analog Çıkış):** -> **Voltaj Bölücüye Girecek**
    * *Voltaj Bölücü Formülü:*
    * MQ-3 A0 Çıkışı ---[2k Direnç]---(Ölçüm Noktası)---[3.3k Direnç]---GND
    * *(Ölçüm Noktası)* -> **ESP32 GPIO (Örn: GPIO 4)**

### Fan ve Butonlar
* **Fan:** Doğrudan 5V'a bağlanabilir (Sürekli çalışır) veya bir MOSFET (IRL540) ile ESP32 tarafından kontrol edilebilir.

---

## 🛠️ 3. Geliştirme Yol Haritası (Roadmap)

### Aşama 1: "Hello World" (Prototip)
- [ ] ESP32-S3 kartına LVGL veya Web Server kurulumu.
- [ ] Ekrana basit bir "Merhaba" yazısı ve buton koyma.
- [ ] Dokunmatik hassasiyetinin test edilmesi (Pleksi arkası testleri).

### Aşama 2: Sensör Kalibrasyonu (En Kritik Aşama)
- [ ] MQ-3 sensörünü bağla ve "Raw Analog" veriyi ekrana yazdır.
- [ ] **Burn-in:** Yeni sensörü en az 24 saat boşta çalıştır (Isıtıcısı yansın).
- [ ] **Base Value:** Temiz havadaki değeri kaydet.
- [ ] **Test:** Bilinen bir alkol oranıyla (veya alkollü nefesle) eşik değerleri belirle.
  - *Örn: Analog 1200 = 0 Promil, Analog 2400 = 0.50 Promil (Tahmini)*

### Aşama 3: Kasa ve Montaj
- [ ] Kasa tasarımı yapılırken **"Air Gap"** (Hava Boşluğu) kuralına uyulmalı.
- [ ] Ekran ile ön pleksi arasında boşluk kalmamalı.
- [ ] **Hava Akışı:** Üflenen nefesin cihazın içinde birikmemesi için arkada tahliye delikleri olmalı.

### Aşama 4: Yazılım Kilitleme (Güvenlik)
- [ ] Yazılım bittikten sonra ESP32 **Flash Encryption** aktif edilmeli.
- [ ] Cihazın USB portundan kod çekilmesi engellenmeli.

---

## 🚨 4. Yazılım Yapısı ve Öneriler

### Arayüz Teknolojisi Seçimi
Bu proje için iki yolun var:

1.  **SquareLine Studio (LVGL):** * *Avantajı:* Çok hızlı, akıcı, profesyonel görünür. C++ ile derlenir.
    * *Dezavantajı:* Öğrenme eğrisi biraz yüksektir.

2.  **Web Tabanlı Kiosk (Verdiğimiz HTML Örneği):**
    * *Yöntem:* ESP32'yi bir Web Sunucusu yaparsın. Ekranda bir WebView (veya basit tarayıcı) çalıştırırsın.
    * *Avantajı:* HTML/CSS ile tasarım yapmak çok kolaydır.
    * *Dezavantajı:* LVGL kadar "native" ve hızlı hissettirmeyebilir.

### QR Kod Stratejisi
QR kodu statik bir resim olarak koyma. `LVGL` kütüphanesinin **QR Code Widget**'ını kullan. Böylece cihazın içinden linki istediğin zaman değiştirebilirsin (Config dosyasından).

---

## ⚠️ 5. Yasal Uyarı ve Sorumluluk Reddi (Disclaimer)

Bu metni cihazın açılış ekranına ve kasasına ekle:

> "Bu cihaz (AlkoMetric) yalnızca eğlence ve bilgilendirme amaçlı tasarlanmıştır. Cihazın ölçüm sonuçları; ortam koşulları, sensör durumu ve kullanım şekline göre değişiklik gösterebilir. Sonuçların yasal, tıbbi veya adli bir geçerliliği yoktur. Sürüş yeterliliğinin tespiti için kullanılamaz. Alkol aldıysanız, lütfen araç kullanmayınız. Üretici ve işletme, doğabilecek olumsuz durumlardan sorumlu tutulamaz."