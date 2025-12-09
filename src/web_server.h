/**
 * ============================================================
 * VibeGo - Web Admin Panel v2.0
 * ============================================================
 * Dashboard with WiFi Settings, Device Settings, and Stats
 * Works in AP+STA mode - Always accessible via AP
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
#include "wifi_handler.h"
#include "sponsor_manager.h"
#include "ui/ui.h"

#define WEB_PORT 80

static AsyncWebServer server(WEB_PORT);
static bool web_server_started = false;
static bool pending_wifi_connect = false;
static String pending_ssid = "";
static String pending_password = "";

// ============================================================
// HTML Dashboard with WiFi Settings
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
        .tabs {
            display: flex;
            background: rgba(0,0,0,0.2);
            border-bottom: 1px solid rgba(255,255,255,0.1);
        }
        .tab {
            flex: 1;
            padding: 12px;
            text-align: center;
            cursor: pointer;
            color: #888;
            font-size: 14px;
            transition: all 0.3s;
        }
        .tab:hover { color: #00d4aa; }
        .tab.active { 
            color: #00d4aa; 
            border-bottom: 2px solid #00d4aa;
            background: rgba(0,212,170,0.1);
        }
        .container { padding: 15px; max-width: 800px; margin: 0 auto; }
        .tab-content { display: none; }
        .tab-content.active { display: block; }
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
        .btn.secondary { background: #4a4a6a; color: #fff; }
        .setting-row { display: flex; align-items: center; justify-content: space-between; padding: 12px 0; border-bottom: 1px solid rgba(255,255,255,0.05); }
        .setting-row .label { color: #eee; font-size: 14px; }
        .setting-row .value { color: #00d4aa; }
        input[type="range"] { width: 150px; }
        input[type="text"], input[type="password"] {
            background: #2a2a4e;
            color: #eee;
            border: 1px solid #444;
            padding: 10px;
            border-radius: 5px;
            width: 100%;
            margin-bottom: 10px;
        }
        select { background: #2a2a4e; color: #eee; border: 1px solid #444; padding: 8px; border-radius: 5px; }
        .footer { text-align: center; padding: 15px; color: #666; font-size: 12px; }
        .btn-group { display: flex; flex-wrap: wrap; gap: 10px; margin-top: 10px; }
        .wifi-network {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 12px;
            background: rgba(255,255,255,0.03);
            border-radius: 8px;
            margin-bottom: 8px;
            cursor: pointer;
            transition: all 0.2s;
        }
        .wifi-network:hover { background: rgba(0,212,170,0.1); }
        .wifi-network .name { font-weight: bold; }
        .wifi-network .signal { color: #888; font-size: 12px; }
        .wifi-network.connected { border: 1px solid #00d4aa; }
        .wifi-form { display: none; margin-top: 15px; padding: 15px; background: rgba(0,0,0,0.2); border-radius: 8px; }
        .wifi-form.active { display: block; }
        .status-badge {
            display: inline-block;
            padding: 4px 10px;
            border-radius: 20px;
            font-size: 12px;
            font-weight: bold;
        }
        .status-badge.connected { background: rgba(0,212,170,0.2); color: #00d4aa; }
        .status-badge.ap-only { background: rgba(255,193,7,0.2); color: #ffc107; }
        .loading { opacity: 0.5; pointer-events: none; }
        .period-selector { display: flex; gap: 5px; margin-bottom: 15px; flex-wrap: wrap; }
        .period-btn {
            background: rgba(255,255,255,0.1);
            border: 1px solid rgba(255,255,255,0.2);
            color: #888;
            padding: 8px 12px;
            border-radius: 20px;
            cursor: pointer;
            font-size: 12px;
            transition: all 0.2s;
        }
        .period-btn:hover { background: rgba(0,212,170,0.2); color: #00d4aa; }
        .period-btn.active { background: #00d4aa; color: #1a1a2e; border-color: #00d4aa; }
        @media (max-width: 480px) {
            .stats-grid { grid-template-columns: repeat(2, 1fr); }
            .period-selector { justify-content: center; }
        }
    </style>
</head>
<body>
    <div class="header">
        <h1>VIBEGO</h1>
    </div>
    
    <div class="tabs">
        <div class="tab active" onclick="showTab('dashboard')">📊 Dashboard</div>
        <div class="tab" onclick="showTab('wifi')">📶 WiFi</div>
        <div class="tab" onclick="showTab('sponsor')">🎯 Sponsor</div>
        <div class="tab" onclick="showTab('settings')">⚙️ Ayarlar</div>
    </div>
    
    <div class="container">
        <!-- Dashboard Tab -->
        <div id="tab-dashboard" class="tab-content active">
            <div class="card">
                <h2>📊 İstatistikler</h2>
                <div class="period-selector">
                    <button class="period-btn active" onclick="setPeriod(0)">Bugün</button>
                    <button class="period-btn" onclick="setPeriod(1)">Hafta</button>
                    <button class="period-btn" onclick="setPeriod(2)">Ay</button>
                    <button class="period-btn" onclick="setPeriod(3)">6 Ay</button>
                    <button class="period-btn" onclick="setPeriod(4)">Tümü</button>
                </div>
                <div class="stats-grid">
                    <div class="stat-box"><div class="value" id="totalTests">-</div><div class="label">Toplam</div></div>
                    <div class="stat-box"><div class="value" id="safeCount">-</div><div class="label">Güvenli</div></div>
                    <div class="stat-box danger"><div class="value" id="dangerCount">-</div><div class="label">Tehlikeli</div></div>
                    <div class="stat-box"><div class="value" id="safePercent">-</div><div class="label">Oran</div></div>
                </div>
                <div class="stats-extra" style="margin-top:10px; display:flex; gap:10px;">
                    <div class="stat-box" style="flex:1"><div class="value" id="avgBac">-</div><div class="label">Ortalama BAC</div></div>
                    <div class="stat-box" style="flex:1"><div class="value" id="maxBac">-</div><div class="label">Maks BAC</div></div>
                </div>
            </div>
            
            <div class="card">
                <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:12px; border-bottom:1px solid rgba(255,255,255,0.1); padding-bottom:8px;">
                    <h2 style="margin:0; border:none; padding:0;">📋 Test Kayıtları</h2>
                    <button class="btn" onclick="exportCSV()" style="margin:0;">📥 CSV İndir</button>
                </div>
                <div style="max-height:300px; overflow-y:auto;">
                    <table>
                        <thead><tr><th>#</th><th>Tarih/Saat</th><th>BAC</th><th>Sonuç</th></tr></thead>
                        <tbody id="logsTable"><tr><td colspan="4">Yükleniyor...</td></tr></tbody>
                    </table>
                </div>
            </div>
            
            <div class="card">
                <h2>ℹ️ Cihaz Bilgisi</h2>
                <div class="info-row"><span class="label">AP IP</span><span id="apIp">-</span></div>
                <div class="info-row"><span class="label">STA IP</span><span id="staIp">-</span></div>
                <div class="info-row"><span class="label">Firmware</span><span id="firmware">-</span></div>
                <div class="info-row"><span class="label">Uptime</span><span id="uptime">-</span></div>
                <div class="info-row"><span class="label">WiFi Durumu</span><span id="wifiStatus">-</span></div>
                <div class="info-row"><span class="label">LDR</span><span id="ldr">-</span></div>
            </div>
            
            <button class="btn" onclick="loadData()" style="width:100%">🔄 Yenile</button>
        </div>
        
        <!-- WiFi Tab -->
        <div id="tab-wifi" class="tab-content">
            <div class="card">
                <h2>📶 WiFi Durumu</h2>
                <div class="info-row">
                    <span class="label">AP Durumu</span>
                    <span id="apStatus">-</span>
                </div>
                <div class="info-row" id="apTimeoutRow">
                    <span class="label">AP Kalan Süre</span>
                    <span id="apTimeout">-</span>
                </div>
                <div class="info-row">
                    <span class="label">AP SSID</span>
                    <span>VibeGo (şifre: 12345678)</span>
                </div>
                <div class="info-row">
                    <span class="label">AP IP</span>
                    <span id="wifiApIp">192.168.4.1</span>
                </div>
                <div class="info-row">
                    <span class="label">Bağlı Ağ</span>
                    <span id="connectedSsid">-</span>
                </div>
                <div class="info-row">
                    <span class="label">STA IP</span>
                    <span id="wifiStaIp">-</span>
                </div>
                <div class="info-row">
                    <span class="label">Sinyal</span>
                    <span id="wifiSignal">-</span>
                </div>
                <div class="btn-group" style="margin-top: 10px;">
                    <button class="btn secondary" onclick="restartAp()">📶 AP'yi Yeniden Başlat</button>
                </div>
            </div>
            
            <div class="card">
                <h2>📡 Ağları Tara ve Bağlan</h2>
                <button class="btn" onclick="scanNetworks()">🔍 Ağları Tara</button>
                <div id="networkList" style="margin-top: 15px;"></div>
                
                <div id="wifiForm" class="wifi-form">
                    <h3 style="margin-bottom: 10px; color: #00d4aa;">Bağlan: <span id="selectedSsid"></span></h3>
                    <input type="text" id="wifiSsid" placeholder="SSID" readonly>
                    <input type="password" id="wifiPassword" placeholder="Şifre">
                    <div class="btn-group">
                        <button class="btn" onclick="connectWifi()">🔗 Bağlan</button>
                        <button class="btn secondary" onclick="hideWifiForm()">İptal</button>
                    </div>
                </div>
            </div>
            
            <div class="card">
                <h2>⚠️ WiFi Ayarlarını Sıfırla</h2>
                <p style="color: #888; font-size: 13px; margin-bottom: 10px;">Kayıtlı ağ bilgilerini siler, sadece AP modunda çalışır.</p>
                <button class="btn danger" onclick="resetWifi()">🗑️ WiFi Ayarlarını Sil</button>
            </div>
        </div>
        
        <!-- Sponsor Tab -->
        <div id="tab-sponsor" class="tab-content">
            <div class="card">
                <h2>📊 QR Tarama İstatistikleri</h2>
                <div class="stats-grid" style="grid-template-columns: repeat(2, 1fr);">
                    <div class="stat-box">
                        <div class="value" id="qrScanCount">0</div>
                        <div class="label">Toplam Tarama</div>
                    </div>
                    <div class="stat-box">
                        <div class="value" id="sponsorName">-</div>
                        <div class="label">Aktif Sponsor</div>
                    </div>
                </div>
                <button class="btn secondary" onclick="resetScanCount()" style="margin-top:10px;">🔄 Sayacı Sıfırla</button>
            </div>
            
            <div class="card">
                <h2>🎯 Sponsor Ayarları</h2>
                <div class="setting-row" style="flex-direction: column; align-items: stretch;">
                    <span class="label" style="margin-bottom: 8px;">QR Yönlendirme URL</span>
                    <input type="text" id="qrUrl" placeholder="https://..." style="margin-bottom: 0;">
                </div>
                <p style="color: #666; font-size: 11px; margin: 8px 0;">Kullanıcı QR'ı taradığında bu adrese yönlendirilir. Uber, BiTaksi veya taksi telefon linki olabilir.</p>
                
                <div class="setting-row" style="flex-direction: column; align-items: stretch; margin-top: 15px;">
                    <span class="label" style="margin-bottom: 8px;">Sponsor Adı</span>
                    <input type="text" id="sponsorNameInput" placeholder="Örn: Kadıköy Taksi" style="margin-bottom: 0;">
                </div>
                
                <div class="setting-row" style="flex-direction: column; align-items: stretch; margin-top: 15px;">
                    <span class="label" style="margin-bottom: 8px;">Sponsor Telefon (opsiyonel)</span>
                    <input type="text" id="sponsorPhone" placeholder="Örn: 0216 XXX XX XX" style="margin-bottom: 0;">
                </div>
                
                <div class="setting-row" style="flex-direction: column; align-items: stretch; margin-top: 15px;">
                    <span class="label" style="margin-bottom: 8px;">QR Başlık (cihaz ekranı)</span>
                    <input type="text" id="qrTitle" placeholder="Örn: TAKSI CAG" style="margin-bottom: 0;">
                </div>
                <p style="color: #666; font-size: 11px; margin: 8px 0;">Cihaz ekranında QR'ın üstünde görünen metin.</p>
                
                <button class="btn" onclick="saveSponsor()" style="width: 100%; margin-top: 15px;">💾 Kaydet</button>
            </div>
            
            <div class="card">
                <h2>💡 Hızlı Linkler</h2>
                <p style="color: #888; font-size: 12px; margin-bottom: 10px;">Popüler taksi uygulamaları için hazır linkler:</p>
                <div class="btn-group">
                    <button class="btn secondary" onclick="setQuickLink('uber')">🚗 Uber</button>
                    <button class="btn secondary" onclick="setQuickLink('bitaksi')">🚕 BiTaksi</button>
                    <button class="btn secondary" onclick="setQuickLink('bolt')">⚡ Bolt</button>
                </div>
            </div>
        </div>
        
        <!-- Settings Tab -->
        <div id="tab-settings" class="tab-content">
            <div class="card">
                <h2>💡 Ekran Ayarları</h2>
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
            </div>
            
            <div class="card">
                <h2>☁️ Cloud Sync</h2>
                <div class="setting-row">
                    <span class="label">Sync Süresi</span>
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
            </div>
            
            <div class="card">
                <h2>🔧 Sistem</h2>
                <div class="btn-group">
                    <button class="btn" onclick="checkUpdate()">🔄 Güncelleme Kontrol</button>
                    <button class="btn" onclick="clearLogs()">🗑️ Logları Sil</button>
                    <button class="btn danger" onclick="rebootDevice()">⚡ Yeniden Başlat</button>
                </div>
            </div>
        </div>
    </div>
    
    <div class="footer">VibeGo v1.3.0 | © 2024 GoLabs</div>
    
    <script>
        let currentPeriod = 0; // 0=today, 1=week, 2=month, 3=6months, 4=all
        
        function showTab(name) {
            document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
            document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
            document.querySelector(`[onclick="showTab('${name}')"]`).classList.add('active');
            document.getElementById('tab-' + name).classList.add('active');
            if (name === 'wifi') loadWifiStatus();
            if (name === 'sponsor') loadSponsor();
        }
        
        function setPeriod(period) {
            currentPeriod = period;
            document.querySelectorAll('.period-btn').forEach((b, i) => {
                b.classList.toggle('active', i === period);
            });
            loadData();
        }
        
        async function loadData() {
            try {
                const [stats, logs, info] = await Promise.all([
                    fetch('/api/stats?period=' + currentPeriod).then(r => r.json()),
                    fetch('/api/logs?period=' + currentPeriod + '&limit=100').then(r => r.json()),
                    fetch('/api/info').then(r => r.json())
                ]);
                
                document.getElementById('totalTests').textContent = stats.total_tests;
                document.getElementById('safeCount').textContent = stats.safe_count;
                document.getElementById('dangerCount').textContent = stats.danger_count;
                document.getElementById('safePercent').textContent = stats.safe_percent + '%';
                document.getElementById('avgBac').textContent = stats.avg_bac;
                document.getElementById('maxBac').textContent = stats.max_bac;
                
                const tbody = document.getElementById('logsTable');
                tbody.innerHTML = logs.logs.length === 0 ? '<tr><td colspan="4">Bu dönemde test yok</td></tr>' :
                    logs.logs.map(l => `<tr><td>${l.id}</td><td>${l.datetime}</td><td>${l.bac.toFixed(2)}</td><td class="${l.safe?'safe':'danger'}">${l.safe?'✓':'✗'}</td></tr>`).join('');
                
                document.getElementById('apIp').textContent = info.ap_ip || '-';
                document.getElementById('staIp').textContent = info.sta_ip || '-';
                document.getElementById('firmware').textContent = 'v' + info.version;
                document.getElementById('uptime').textContent = info.uptime;
                document.getElementById('wifiStatus').innerHTML = info.wifi_connected ? 
                    `<span class="status-badge connected">Bağlı - ${info.ssid}</span>` :
                    `<span class="status-badge ap-only">Sadece AP</span>`;
                document.getElementById('ldr').textContent = info.ldr;
                document.getElementById('brightnessSlider').value = info.brightness_raw;
                document.getElementById('brightnessVal').textContent = info.brightness + '%';
                document.getElementById('autoBrightness').value = info.auto_brightness ? '1' : '0';
                document.getElementById('syncInterval').value = info.sync_interval || '0';
                document.getElementById('pendingCount').textContent = info.pending_count || '0';
            } catch (e) { console.error(e); }
        }
        
        async function loadWifiStatus() {
            try {
                const info = await fetch('/api/wifi_status').then(r => r.json());
                document.getElementById('apStatus').innerHTML = info.ap_active ? 
                    '<span class="status-badge connected">Aktif</span>' : 
                    '<span class="status-badge ap-only">Kapalı</span>';
                document.getElementById('apTimeoutRow').style.display = info.ap_active ? 'flex' : 'none';
                if (info.ap_active && info.ap_remaining > 0) {
                    const mins = Math.floor(info.ap_remaining / 60);
                    const secs = info.ap_remaining % 60;
                    document.getElementById('apTimeout').textContent = mins + ' dk ' + secs + ' sn';
                } else if (info.ap_active) {
                    document.getElementById('apTimeout').textContent = 'Süresiz (bağlantı bekleniyor)';
                }
                document.getElementById('wifiApIp').textContent = info.ap_active ? info.ap_ip : '-';
                document.getElementById('connectedSsid').textContent = info.connected ? info.ssid : '-';
                document.getElementById('wifiStaIp').textContent = info.connected ? info.sta_ip : '-';
                document.getElementById('wifiSignal').textContent = info.connected ? info.signal + '%' : '-';
            } catch(e) { console.error(e); }
        }
        
        async function restartAp() {
            await fetch('/api/wifi_restart_ap');
            alert('AP yeniden başlatıldı (30 dk aktif)');
            loadWifiStatus();
        }
        
        async function scanNetworks() {
            const list = document.getElementById('networkList');
            list.innerHTML = '<div style="color:#888">Taranıyor...</div>';
            try {
                const data = await fetch('/api/wifi_scan').then(r => r.json());
                if (data.networks.length === 0) {
                    list.innerHTML = '<div style="color:#888">Ağ bulunamadı</div>';
                    return;
                }
                list.innerHTML = data.networks.map(n => 
                    `<div class="wifi-network" onclick="selectNetwork('${n.ssid}', ${n.open})">
                        <div><div class="name">${n.ssid}</div><div class="signal">${n.open ? '🔓 Açık' : '🔒 Şifreli'}</div></div>
                        <div class="signal">${getSignalIcon(n.rssi)} ${n.rssi} dBm</div>
                    </div>`
                ).join('');
            } catch(e) { 
                list.innerHTML = '<div style="color:#ff5252">Tarama hatası</div>';
            }
        }
        
        function getSignalIcon(rssi) {
            if (rssi > -50) return '📶';
            if (rssi > -70) return '📶';
            return '📶';
        }
        
        function selectNetwork(ssid, isOpen) {
            document.getElementById('wifiSsid').value = ssid;
            document.getElementById('selectedSsid').textContent = ssid;
            document.getElementById('wifiPassword').value = '';
            document.getElementById('wifiPassword').style.display = isOpen ? 'none' : 'block';
            document.getElementById('wifiForm').classList.add('active');
        }
        
        function hideWifiForm() {
            document.getElementById('wifiForm').classList.remove('active');
        }
        
        async function connectWifi() {
            const ssid = document.getElementById('wifiSsid').value;
            const password = document.getElementById('wifiPassword').value;
            if (!ssid) return alert('SSID gerekli');
            
            try {
                const res = await fetch('/api/wifi_connect', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({ssid, password})
                });
                const data = await res.json();
                if (data.success) {
                    alert('Bağlantı başarılı! IP: ' + data.ip);
                    hideWifiForm();
                    loadWifiStatus();
                } else {
                    alert('Bağlantı başarısız: ' + (data.error || 'Bilinmeyen hata'));
                }
            } catch(e) {
                alert('Bağlantı hatası');
            }
        }
        
        async function resetWifi() {
            if (confirm('WiFi ayarlarını sıfırlamak istiyor musunuz?')) {
                await fetch('/api/wifi_reset', {method: 'POST'});
                alert('WiFi ayarları silindi');
                loadWifiStatus();
            }
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
            alert('Güncelleme kontrolü başlatıldı.');
            await fetch('/api/check_update');
        }
        
        async function exportCSV() {
            const periodNames = ['bugun', 'hafta', 'ay', '6ay', 'tumu'];
            const response = await fetch('/api/export_csv?period=' + currentPeriod);
            const csv = await response.text();
            
            const blob = new Blob([csv], { type: 'text/csv;charset=utf-8;' });
            const link = document.createElement('a');
            link.href = URL.createObjectURL(blob);
            link.download = 'vibego_testler_' + periodNames[currentPeriod] + '.csv';
            link.click();
        }
        
        // Sponsor Management
        async function loadSponsor() {
            try {
                const data = await fetch('/api/sponsor').then(r => r.json());
                document.getElementById('qrScanCount').textContent = data.scan_count;
                document.getElementById('sponsorName').textContent = data.name || '-';
                document.getElementById('qrUrl').value = data.qr_url || '';
                document.getElementById('sponsorNameInput').value = data.name || '';
                document.getElementById('sponsorPhone').value = data.phone || '';
                document.getElementById('qrTitle').value = data.qr_title || '';
            } catch(e) { console.error(e); }
        }
        
        async function saveSponsor() {
            const data = {
                qr_url: document.getElementById('qrUrl').value,
                name: document.getElementById('sponsorNameInput').value,
                phone: document.getElementById('sponsorPhone').value,
                qr_title: document.getElementById('qrTitle').value
            };
            try {
                await fetch('/api/sponsor', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify(data)
                });
                alert('Sponsor ayarları kaydedildi!');
                loadSponsor();
            } catch(e) {
                alert('Kaydetme hatası!');
            }
        }
        
        function setQuickLink(app) {
            const links = {
                'uber': 'https://m.uber.com/ul/?action=setPickup&pickup=my_location',
                'bitaksi': 'https://www.bitaksi.com/',
                'bolt': 'https://bolt.eu/'
            };
            document.getElementById('qrUrl').value = links[app] || '';
            document.getElementById('sponsorNameInput').value = app.charAt(0).toUpperCase() + app.slice(1);
        }
        
        async function resetScanCount() {
            if (confirm('Tarama sayacını sıfırlamak istiyor musunuz?')) {
                await fetch('/api/sponsor/reset_scans', {method: 'POST'});
                loadSponsor();
            }
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
    StaticJsonDocument<512> doc;
    doc["ap_ip"] = WiFi.softAPIP().toString();
    doc["sta_ip"] = WiFi.isConnected() ? WiFi.localIP().toString() : "";
    doc["version"] = http_ota_get_version();
    doc["uptime"] = ntp_get_uptime_str();
    doc["wifi_connected"] = WiFi.isConnected();
    doc["ssid"] = WiFi.isConnected() ? WiFi.SSID() : "";
    doc["rssi"] = WiFi.isConnected() ? (WiFi.RSSI() + 100) * 2 : 0;
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

String web_get_wifi_status_json() {
    StaticJsonDocument<320> doc;
    doc["ap_ip"] = wifi_is_ap_active() ? WiFi.softAPIP().toString() : "";
    doc["sta_ip"] = WiFi.isConnected() ? WiFi.localIP().toString() : "";
    doc["connected"] = WiFi.isConnected();
    doc["ssid"] = WiFi.isConnected() ? WiFi.SSID() : "";
    doc["signal"] = wifi_get_signal_percent();
    doc["ap_clients"] = wifi_get_ap_clients();
    doc["saved_ssid"] = wifi_get_saved_ssid();
    doc["ap_active"] = wifi_is_ap_active();
    doc["ap_remaining"] = wifi_get_ap_remaining_seconds();
    
    String output;
    serializeJson(doc, output);
    return output;
}

// ============================================================
// Server Init
// ============================================================
void web_server_init() {
    if (web_server_started) return;
    
    Serial.println("[WEB] Starting server...");
    
    // Dashboard
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send_P(200, "text/html", DASHBOARD_HTML);
    });
    
    // Stats API with period filter
    server.on("/api/stats", HTTP_GET, [](AsyncWebServerRequest *r) {
        filter_period_t period = FILTER_TODAY;
        if (r->hasParam("period")) {
            period = (filter_period_t)r->getParam("period")->value().toInt();
        }
        r->send(200, "application/json", logger_get_stats_json_filtered(period));
    });
    
    // Logs API with period filter
    server.on("/api/logs", HTTP_GET, [](AsyncWebServerRequest *r) {
        filter_period_t period = FILTER_ALL;
        int limit = 100;
        if (r->hasParam("period")) {
            period = (filter_period_t)r->getParam("period")->value().toInt();
        }
        if (r->hasParam("limit")) {
            limit = r->getParam("limit")->value().toInt();
        }
        r->send(200, "application/json", logger_get_logs_json_filtered(period, limit));
    });
    
    // CSV Export API
    server.on("/api/export_csv", HTTP_GET, [](AsyncWebServerRequest *r) {
        filter_period_t period = FILTER_ALL;
        if (r->hasParam("period")) {
            period = (filter_period_t)r->getParam("period")->value().toInt();
        }
        r->send(200, "text/csv", logger_export_csv(period));
    });
    
    // Device Info API
    server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", web_get_info_json());
    });
    
    // WiFi Status API
    server.on("/api/wifi_status", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", web_get_wifi_status_json());
    });
    
    // WiFi Scan API
    server.on("/api/wifi_scan", HTTP_GET, [](AsyncWebServerRequest *r) {
        wifi_scan_networks();
        r->send(200, "application/json", wifi_get_networks_json());
    });
    
    // WiFi Connect API
    server.on("/api/wifi_connect", HTTP_POST, [](AsyncWebServerRequest *r) {},
        NULL,
        [](AsyncWebServerRequest *r, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<256> doc;
            DeserializationError error = deserializeJson(doc, data, len);
            
            if (error) {
                r->send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
                return;
            }
            
            String ssid = doc["ssid"] | "";
            String password = doc["password"] | "";
            
            if (ssid.length() == 0) {
                r->send(400, "application/json", "{\"success\":false,\"error\":\"SSID required\"}");
                return;
            }
            
            // Try to connect
            if (wifi_connect(ssid, password, true)) {
                String response = "{\"success\":true,\"ip\":\"" + WiFi.localIP().toString() + "\"}";
                r->send(200, "application/json", response);
            } else {
                r->send(200, "application/json", "{\"success\":false,\"error\":\"Connection failed\"}");
            }
        }
    );
    
    // WiFi Reset API
    server.on("/api/wifi_reset", HTTP_POST, [](AsyncWebServerRequest *r) {
        wifi_clear_credentials();
        WiFi.disconnect();
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // WiFi Restart AP API
    server.on("/api/wifi_restart_ap", HTTP_GET, [](AsyncWebServerRequest *r) {
        wifi_restart_ap();
        Serial.println("[WEB] AP restarted via web");
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Clear Logs API
    server.on("/api/clear", HTTP_POST, [](AsyncWebServerRequest *r) {
        logger_clear();
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Sync Interval API
    server.on("/api/sync_interval", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("value")) {
            int val = r->getParam("value")->value().toInt();
            webhook_set_interval(val);
            Serial.printf("[WEB] Sync interval: %d min\n", val);
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Sync Now API
    server.on("/api/sync_now", HTTP_GET, [](AsyncWebServerRequest *r) {
        int synced = webhook_sync_now();
        Serial.printf("[WEB] Manual sync: %d entries\n", synced);
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Brightness API
    server.on("/api/brightness", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("value")) {
            int val = r->getParam("value")->value().toInt();
            brightness_set_manual(val);
            Serial.printf("[WEB] Brightness set to %d\n", val);
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Auto Brightness API
    server.on("/api/auto_brightness", HTTP_GET, [](AsyncWebServerRequest *r) {
        if (r->hasParam("enabled")) {
            bool enabled = r->getParam("enabled")->value() == "1";
            brightness_set_auto(enabled);
            Serial.printf("[WEB] Auto brightness: %s\n", enabled ? "ON" : "OFF");
        }
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // Reboot API
    server.on("/api/reboot", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", "{\"ok\":true}");
        delay(500);
        ESP.restart();
    });
    
    // Check Update API
    server.on("/api/check_update", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", "{\"ok\":true}");
        http_ota_force_update();
    });
    
    // Sponsor Get API
    server.on("/api/sponsor", HTTP_GET, [](AsyncWebServerRequest *r) {
        r->send(200, "application/json", sponsor_get_json());
    });
    
    // Sponsor Save API
    server.on("/api/sponsor", HTTP_POST, [](AsyncWebServerRequest *r) {},
        NULL,
        [](AsyncWebServerRequest *r, uint8_t *data, size_t len, size_t index, size_t total) {
            StaticJsonDocument<320> doc;
            DeserializationError error = deserializeJson(doc, data, len);
            
            if (error) {
                r->send(400, "application/json", "{\"ok\":false}");
                return;
            }
            
            String qr_url = doc["qr_url"] | "";
            String name = doc["name"] | "";
            String phone = doc["phone"] | "";
            String qr_title = doc["qr_title"] | "";
            
            if (qr_url.length() > 0) {
                sponsor_set_qr_url(qr_url);
                // Update QR code on device screen
                ui_update_qr_code(qr_url.c_str());
            }
            if (name.length() > 0) sponsor_set_name(name);
            sponsor_set_phone(phone);
            
            if (qr_title.length() > 0) {
                sponsor_set_qr_title(qr_title);
                // Update QR title on device screen
                ui_update_qr_title(qr_title.c_str());
            }
            
            Serial.printf("[WEB] Sponsor updated: %s\n", name.c_str());
            r->send(200, "application/json", "{\"ok\":true}");
        }
    );
    
    // Sponsor Reset Scans API
    server.on("/api/sponsor/reset_scans", HTTP_POST, [](AsyncWebServerRequest *r) {
        sponsor_reset_scans();
        r->send(200, "application/json", "{\"ok\":true}");
    });
    
    // QR Redirect Page (for tracking scans)
    server.on("/go", HTTP_GET, [](AsyncWebServerRequest *r) {
        sponsor_increment_scan();
        String redirectUrl = sponsor_get_qr_url();
        String html = "<html><head><meta http-equiv='refresh' content='0;url=" + redirectUrl + "'></head>";
        html += "<body>Yonlendiriliyor...</body></html>";
        r->send(200, "text/html", html);
    });
    
    server.begin();
    web_server_started = true;
    
    Serial.printf("[WEB] AP Dashboard: http://%s/\n", WiFi.softAPIP().toString().c_str());
    if (WiFi.isConnected()) {
        Serial.printf("[WEB] STA Dashboard: http://%s/\n", WiFi.localIP().toString().c_str());
    }
}

bool web_server_is_running() { return web_server_started; }

#endif
