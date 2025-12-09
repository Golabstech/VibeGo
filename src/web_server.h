/**
 * ============================================================
 * VibeGo - Web Admin Panel
 * ============================================================
 * Dashboard with Settings Controls
 * ============================================================
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "ntp_time.h"
#include "data_logger.h"
#include "http_ota.h"
#include "brightness.h"

#define WEB_PORT 80

static AsyncWebServer server(WEB_PORT);
static bool web_server_started = false;

// ============================================================
// HTML Dashboard with Settings
// ============================================================
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VibeGo Dashboard</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
            min-height: 100vh;
            color: #eee;
        }
        .header {
            background: rgba(0,0,0,0.3);
            padding: 15px;
            text-align: center;
            border-bottom: 2px solid #00d4aa;
        }
        .header h1 { color: #00d4aa; font-size: 24px; letter-spacing: 3px; }
        .container { padding: 15px; max-width: 800px; margin: 0 auto; }
        .card {
            background: rgba(255,255,255,0.05);
            border-radius: 12px;
            padding: 15px;
            margin-bottom: 15px;
            border: 1px solid rgba(255,255,255,0.1);
        }
        .card h2 { 
            color: #00d4aa; 
            margin-bottom: 12px;
            font-size: 16px;
            border-bottom: 1px solid rgba(255,255,255,0.1);
            padding-bottom: 8px;
        }
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(4, 1fr);
            gap: 10px;
        }
        .stat-box {
            background: rgba(0,212,170,0.1);
            border-radius: 8px;
            padding: 12px;
            text-align: center;
        }
        .stat-box .value { font-size: 24px; font-weight: bold; color: #00d4aa; }
        .stat-box .label { color: #888; font-size: 11px; margin-top: 3px; }
        .stat-box.danger { background: rgba(255,82,82,0.1); }
        .stat-box.danger .value { color: #ff5252; }
        table { width: 100%; border-collapse: collapse; font-size: 13px; }
        th, td { padding: 8px; text-align: left; border-bottom: 1px solid rgba(255,255,255,0.1); }
        th { color: #00d4aa; font-size: 11px; }
        .safe { color: #00d4aa; }
        .danger { color: #ff5252; }
        .info-row { display: flex; justify-content: space-between; padding: 8px 0; border-bottom: 1px solid rgba(255,255,255,0.05); font-size: 13px; }
        .info-row .label { color: #888; }
        .btn {
            background: #00d4aa;
            color: #1a1a2e;
            border: none;
            padding: 10px 15px;
            border-radius: 5px;
            cursor: pointer;
            font-weight: bold;
            font-size: 13px;
            margin: 5px;
        }
        .btn:hover { background: #00b894; }
        .btn.danger { background: #ff5252; color: #fff; }
        .btn.danger:hover { background: #e04040; }
        .setting-row { display: flex; align-items: center; justify-content: space-between; padding: 12px 0; border-bottom: 1px solid rgba(255,255,255,0.05); }
        .setting-row .label { color: #eee; font-size: 14px; }
        .setting-row .value { color: #00d4aa; }
        input[type="range"] { width: 150px; }
        select { background: #2a2a4e; color: #eee; border: 1px solid #444; padding: 8px; border-radius: 5px; }
        .footer { text-align: center; padding: 15px; color: #666; font-size: 12px; }
        .btn-group { display: flex; flex-wrap: wrap; gap: 10px; margin-top: 10px; }
    </style>
</head>
<body>
    <div class="header">
        <h1>VIBEGO</h1>
    </div>
    
    <div class="container">
        <!-- Stats -->
        <div class="card">
            <h2>📊 Günlük İstatistikler</h2>
            <div class="stats-grid">
                <div class="stat-box"><div class="value" id="totalTests">-</div><div class="label">Toplam</div></div>
                <div class="stat-box"><div class="value" id="safeCount">-</div><div class="label">Güvenli</div></div>
                <div class="stat-box danger"><div class="value" id="dangerCount">-</div><div class="label">Tehlikeli</div></div>
                <div class="stat-box"><div class="value" id="safePercent">-</div><div class="label">Oran</div></div>
            </div>
        </div>
        
        <!-- Settings -->
        <div class="card">
            <h2>⚙️ Ayarlar</h2>
            <div class="setting-row">
                <span class="label">Parlaklık</span>
                <div>
                    <input type="range" id="brightnessSlider" min="30" max="255" value="255" onchange="setBrightness(this.value)">
                    <span id="brightnessVal">100%</span>
                </div>
            </div>
            <div class="setting-row">
                <span class="label">Otomatik Parlaklık (LDR)</span>
                <select id="autoBrightness" onchange="setAutoBrightness(this.value)">
                    <option value="1">Açık</option>
                    <option value="0">Kapalı</option>
                </select>
            </div>
            <div class="setting-row">
                <span class="label">Cloud Sync Süresi</span>
                <select id="syncInterval" onchange="setSyncInterval(this.value)">
                    <option value="0">Hemen</option>
                    <option value="1">1 Dakika</option>
                    <option value="10">10 Dakika</option>
                    <option value="30">30 Dakika</option>
                    <option value="60">1 Saat</option>
                    <option value="1440">1 Gün</option>
                </select>
            </div>
            <div class="setting-row">
                <span class="label">Bekleyen: <span id="pendingCount">0</span></span>
                <button class="btn" onclick="syncNow()">☁️ Şimdi Sync</button>
            </div>
            <div class="btn-group">
                <button class="btn" onclick="checkUpdate()">🔄 Güncelleme Kontrol</button>
                <button class="btn" onclick="clearLogs()">🗑️ Logları Sil</button>
                <button class="btn danger" onclick="rebootDevice()">⚡ Yeniden Başlat</button>
            </div>
        </div>
        
        <!-- Logs -->
        <div class="card">
            <h2>📋 Son Testler</h2>
            <table>
                <thead><tr><th>#</th><th>Tarih/Saat</th><th>BAC</th><th>Sonuç</th></tr></thead>
                <tbody id="logsTable"><tr><td colspan="4">Yükleniyor...</td></tr></tbody>
            </table>
        </div>
        
        <!-- Device Info -->
        <div class="card">
            <h2>ℹ️ Cihaz Bilgisi</h2>
            <div class="info-row"><span class="label">IP</span><span id="ipAddress">-</span></div>
            <div class="info-row"><span class="label">Firmware</span><span id="firmware">-</span></div>
            <div class="info-row"><span class="label">Uptime</span><span id="uptime">-</span></div>
            <div class="info-row"><span class="label">WiFi</span><span id="rssi">-</span></div>
            <div class="info-row"><span class="label">LDR</span><span id="ldr">-</span></div>
        </div>
        
        <button class="btn" onclick="loadData()" style="width:100%">🔄 Yenile</button>
    </div>
    
    <div class="footer">VibeGo v1.0.0 | © 2024 GoLabs</div>
    
    <script>
        async function loadData() {
            try {
                const [stats, logs, info] = await Promise.all([
                    fetch('/api/stats').then(r => r.json()),
                    fetch('/api/logs').then(r => r.json()),
                    fetch('/api/info').then(r => r.json())
                ]);
                
                document.getElementById('totalTests').textContent = stats.total_tests;
                document.getElementById('safeCount').textContent = stats.safe_count;
                document.getElementById('dangerCount').textContent = stats.danger_count;
                document.getElementById('safePercent').textContent = stats.safe_percent + '%';
                
                const tbody = document.getElementById('logsTable');
                tbody.innerHTML = logs.logs.length === 0 ? '<tr><td colspan="4">Henüz test yok</td></tr>' :
                    logs.logs.map(l => `<tr><td>${l.id}</td><td>${l.datetime}</td><td>${l.bac.toFixed(2)}</td><td class="${l.safe?'safe':'danger'}">${l.safe?'✓':'✗'}</td></tr>`).join('');
                
                document.getElementById('ipAddress').textContent = info.ip;
                document.getElementById('firmware').textContent = 'v' + info.version;
                document.getElementById('uptime').textContent = info.uptime;
                document.getElementById('rssi').textContent = info.rssi + '%';
                document.getElementById('ldr').textContent = info.ldr;
                document.getElementById('brightnessSlider').value = info.brightness_raw;
                document.getElementById('brightnessVal').textContent = info.brightness + '%';
                document.getElementById('autoBrightness').value = info.auto_brightness ? '1' : '0';
                document.getElementById('syncInterval').value = info.sync_interval || '0';
                document.getElementById('pendingCount').textContent = info.pending_count || '0';
            } catch (e) { console.error(e); }
        }
        
        async function setBrightness(val) {
            document.getElementById('brightnessVal').textContent = Math.round(val/255*100) + '%';
            await fetch('/api/brightness?value=' + val);
        }
        
        async function setAutoBrightness(val) {
            await fetch('/api/auto_brightness?enabled=' + val);
        }
        
        async function setSyncInterval(val) {
            await fetch('/api/sync_interval?value=' + val);
        }
        
        async function syncNow() {
            await fetch('/api/sync_now');
            loadData();
        }
        
        async function rebootDevice() {
            if (confirm('Cihazı yeniden başlatmak istiyor musunuz?')) {
                await fetch('/api/reboot');
                alert('Cihaz yeniden başlatılıyor...');
            }
        }
        
        async function clearLogs() {
            if (confirm('Tüm logları silmek istiyor musunuz?')) {
                await fetch('/api/clear', {method:'POST'});
                loadData();
            }
        }
        
        async function checkUpdate() {
            alert('Güncelleme kontrolü başlatıldı. Serial monitörü kontrol edin.');
            await fetch('/api/check_update');
        }
        
        loadData();
        setInterval(loadData, 30000);
    </script>
</body>
</html>
)rawliteral";

// ============================================================
// API Handlers
// ============================================================
String web_get_info_json() {
    StaticJsonDocument<384> doc;
    doc["ip"] = WiFi.localIP().toString();
    doc["version"] = http_ota_get_version();
    doc["uptime"] = ntp_get_uptime_str();
    doc["rssi"] = (WiFi.RSSI() + 100) * 2;
    doc["brightness"] = (brightness_get_level() * 100) / 255;
    doc["brightness_raw"] = brightness_get_level();
    doc["auto_brightness"] = brightness_is_auto();
    doc["ldr"] = brightness_get_ldr_raw();
    doc["sync_interval"] = webhook_get_interval();
    doc["pending_count"] = webhook_get_pending_count();
    
    String output;
    serializeJson(doc, output);
    return output;
}

// ============================================================
// Server Init
// ============================================================
void web_server_init() {
    if (web_server_started || !WiFi.isConnected()) return;
    
    Serial.println("[WEB] Starting server...");
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send_P(200, "text/html", DASHBOARD_HTML);
    });
    
    server.on("/api/stats", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", logger_get_stats_json());
    });
    
    server.on("/api/logs", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", logger_get_logs_json(20));
    });
    
    server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", web_get_info_json());
    });
    
    server.on("/api/clear", HTTP_POST, [](AsyncWebServerRequest *r) {
        logger_clear();
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    server.on("/api/sync_interval", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("value")) {
            int val = r->getParam("value")->value().toInt();
            webhook_set_interval(val);
            Serial.printf("[WEB] Sync interval: %d min\n", val);
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    server.on("/api/sync_now", HTTP_GET, [](AsyncWebServerRequest *r) {
        int synced = webhook_sync_now();
        Serial.printf("[WEB] Manual sync: %d entries\n", synced);
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    server.on("/api/brightness", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("value")) {
            int val = r->getParam("value")->value().toInt();
            brightness_set_manual(val);
            Serial.printf("[WEB] Brightness set to %d\n", val);
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    server.on("/api/auto_brightness", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("enabled")) {
            bool enabled = r->getParam("enabled")->value() == "1";
            brightness_set_auto(enabled);
            Serial.printf("[WEB] Auto brightness: %s\n", enabled ? "ON" : "OFF");
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    server.on("/api/reboot", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", "{\"ok\":true}");
        delay(500);
        ESP.restart();
    });
    
    server.on("/api/check_update", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", "{\"ok\":true}");
        http_ota_force_update();
    });
    
    server.begin();
    web_server_started = true;
    Serial.printf("[WEB] http://%s/\n", WiFi.localIP().toString().c_str());
}

bool web_server_is_running() { return web_server_started; }

#endif
