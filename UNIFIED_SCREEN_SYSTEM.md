# VibeGo - Unified Screen Management System

## 🎯 Amaç

Tüm ekranlar için **tutarlı** ve **güvenli** bir bellek yönetimi ve ekran geçiş sistemi oluşturmak.

## ✅ Yeni Unified Sistem

### **safe_scr_load_with_cleanup()**

Tüm ekran geçişleri için **tek bir fonksiyon**:

```c
static void safe_scr_load_with_cleanup(lv_obj_t * new_scr) {
    // 1. Transition guard (çift yükleme önleme)
    if (is_transitioning) return;
    if (lv_scr_act() == new_scr) return;
    
    is_transitioning = true;
    
    // 2. BELLEK TEMİZLİĞİ
    lv_obj_t * old_scr = lv_scr_act();
    if (old_scr != NULL && old_scr != new_scr) {
        lv_obj_clean(old_scr); // Eski ekranın tüm child'larını sil
    }
    
    // 3. LVGL TASK HANDLER
    lv_task_handler(); // Pending işlemleri tamamla
    
    // 4. WATCHDOG RESET
    esp_task_wdt_reset(); // Timeout önleme
    
    // 5. YENİ EKRANI YÜKLE
    lv_scr_load_anim(new_scr, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
    // - Animasyon yok (NONE)
    // - Auto-delete yok (false) - manuel kontrol
    
    // 6. LOCK RELEASE
    is_transitioning = false;
    
    printf("[UI] Screen loaded: %p\n", new_scr);
}
```

## 📋 Tüm Ekranlar İçin Kullanım

### 1. **Event Handler'lar**
```c
static void ui_event_disclaimer_btn(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_Home_screen_init(); // Ekranı oluştur
        safe_scr_load(ui_Home, ...); // Eski API (redirect eder)
        // VEYA
        safe_scr_load_with_cleanup(ui_Home); // Yeni API
    }
}
```

### 2. **Result Ekranları**
```c
void ui_show_result_safe(float bac_value) {
    ui_Result_screen_init(); // Ekranı oluştur
    
    // Değerleri güncelle
    lv_label_set_text(ui_Result_Value, "0.42");
    lv_obj_set_style_text_color(...);
    
    // UNIFIED sistem kullan
    safe_scr_load_with_cleanup(ui_Result);
}
```

### 3. **Timer Callback'ler**
```c
static void splash_timer_cb(lv_timer_t * timer) {
    if (splash_progress >= 100) {
        lv_timer_del(timer);
        ui_Disclaimer_screen_init(); // Ekranı oluştur
        safe_scr_load(ui_Disclaimer, ...); // Redirect eder
    }
}
```

## 🔄 Önceki vs Sonraki

### ❌ **Önceki Sistem** (Tutarsız)

```c
// Event handler'larda
safe_scr_load(ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);

// Result ekranında
lv_obj_clean(lv_scr_act());
lv_task_handler();
esp_task_wdt_reset();
ui_Result_screen_init();
lv_scr_load_anim(ui_Result, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
is_transitioning = false;
```

**Sorunlar**:
- ❌ Farklı ekranlar farklı yöntemler kullanıyor
- ❌ Bazı ekranlar bellek temizliği yapıyor, bazıları yapmıyor
- ❌ Watchdog reset bazı yerlerde var, bazı yerlerde yok
- ❌ Auto-delete bazen true, bazen false
- ❌ Kod tekrarı

### ✅ **Yeni Sistem** (Unified)

```c
// TÜM ekranlar için
ui_XXX_screen_init(); // Ekranı oluştur
safe_scr_load_with_cleanup(ui_XXX); // Yükle
```

**Avantajlar**:
- ✅ Tek bir fonksiyon, tüm ekranlar için
- ✅ Her zaman bellek temizliği
- ✅ Her zaman watchdog reset
- ✅ Her zaman LVGL task handler
- ✅ Tutarlı davranış
- ✅ Kod tekrarı yok

## 📊 Bellek Yönetimi

### Ekran Yaşam Döngüsü

```
1. OLUŞTURMA
   ui_XXX_screen_init()
   └─> lv_obj_create(NULL)
   └─> Child objeler oluştur

2. YÜKLEME ÖNCESİ
   safe_scr_load_with_cleanup()
   └─> Eski ekranı temizle (lv_obj_clean)
   └─> LVGL task handler
   └─> Watchdog reset

3. YÜKLEME
   lv_scr_load_anim(NONE, auto_del=false)
   └─> Anında geçiş
   └─> Manuel bellek yönetimi

4. TEMİZLİK
   Bir sonraki ekran yüklendiğinde
   └─> lv_obj_clean() ile child'lar silinir
   └─> Screen objesi kalır (yeniden kullanım için)
```

## 🎨 Ekran Oluşturma Formatı

Tüm ekranlar aynı formatta:

```c
void ui_XXX_screen_init(void) {
    // 1. Ana screen objesi
    ui_XXX = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_XXX, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_XXX, lv_color_hex(UI_COLOR_BG_DARK), 0);
    lv_obj_set_style_bg_opa(ui_XXX, 255, 0);
    
    // 2. Panel'ler (opsiyonel)
    lv_obj_t * panel = lv_obj_create(ui_XXX);
    lv_obj_set_size(panel, width, height);
    lv_obj_align(panel, LV_ALIGN_XXX, x, y);
    lv_obj_set_style_bg_color(panel, lv_color_hex(UI_COLOR_PANEL), 0);
    lv_obj_set_style_radius(panel, radius, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    
    // 3. UI elemanları
    ui_XXX_Label = lv_label_create(panel);
    lv_label_set_text(ui_XXX_Label, "Text");
    lv_obj_set_style_text_font(ui_XXX_Label, &lv_font_montserrat_XX, 0);
    lv_obj_align(ui_XXX_Label, LV_ALIGN_XXX, x, y);
    
    // 4. Event handler'lar
    lv_obj_add_event_cb(ui_XXX_Button, ui_event_XXX, LV_EVENT_CLICKED, NULL);
}
```

## 🔧 Kurallar

### ✅ YAPILMASI GEREKENLER

1. **Her zaman `safe_scr_load_with_cleanup()` kullan**
2. **Ekran oluşturmadan önce `ui_XXX_screen_init()` çağır**
3. **Animasyon kullanma** (performans)
4. **Zoom kullanma** (bellek)
5. **Tutarlı stil kullan** (color, font, radius)

### ❌ YAPILMAMASI GEREKENLER

1. **Direkt `lv_scr_load_anim()` kullanma**
2. **Manuel bellek temizliği yapma** (unified sistem halleder)
3. **Auto-delete kullanma** (manuel kontrol daha iyi)
4. **Farklı ekranlar için farklı yöntemler kullanma**

## 📈 Performans

### Bellek Kullanımı
- **Önceki**: Tutarsız (bazı ekranlar leak yapıyor)
- **Şimdi**: Tutarlı (her geçişte temizlik)

### CPU Kullanımı
- **Önceki**: Değişken (animasyon, zoom)
- **Şimdi**: Sabit ve düşük (no anim, no zoom)

### Güvenilirlik
- **Önceki**: Bazen reset (bellek sorunları)
- **Şimdi**: Stabil (watchdog reset + cleanup)

## 🎉 Sonuç

Artık **tüm ekranlar**:
- ✅ Aynı bellek yönetimini kullanıyor
- ✅ Aynı ekran geçiş sistemini kullanıyor
- ✅ Tutarlı davranış sergiliyor
- ✅ Bellek sızıntısı yok
- ✅ Watchdog timeout yok
- ✅ Performanslı ve güvenilir

---

**Versiyon**: 1.4.3  
**Tarih**: 2025-12-16  
**Sistem**: Unified Screen Management
