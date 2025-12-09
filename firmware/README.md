# 🔄 OTA Firmware Update

Bu klasör, uzaktan firmware güncellemesi için kullanılır.

## Yeni Sürüm Yayınlama

1. **Derleyin:**
   ```bash
   pio run
   ```

2. **firmware.bin dosyasını kopyalayın:**
   ```bash
   copy .pio\build\jc2432w328\firmware.bin firmware\
   ```

3. **version.json'u güncelleyin:**
   ```json
   {
       "version": "1.0.1",
       "file": "firmware.bin",
       "url": "https://raw.githubusercontent.com/Golabstech/VibeGo/master/firmware/firmware.bin"
   }
   ```

4. **GitHub'a yükleyin:**
   ```bash
   git add firmware/
   git commit -m "Release v1.0.1"
   git push
   ```

## Güncelleme Nasıl Çalışır?

1. Cihaz WiFi'ye bağlandığında **her saat** version.json kontrol eder
2. Yeni sürüm varsa firmware.bin otomatik indirilir
3. Güncelleme tamamlanınca cihaz yeniden başlar

## Manuel Güncelleme

Serial monitor'dan:
```
update
```
veya
```
ota
```

## Dosyalar

| Dosya | Açıklama |
|-------|----------|
| `version.json` | Güncel versiyon bilgisi |
| `firmware.bin` | Derlenmiş firmware dosyası |
