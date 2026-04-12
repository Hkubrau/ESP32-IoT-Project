# 🌡️ ESP32 Ortam İzleme Sistemi

## 📌 Proje Açıklaması

Bu proje, ESP32 mikrodenetleyicisi kullanılarak ortamın **sıcaklık**, **nem** ve **ışık yoğunluğu** değerlerini sürekli olarak izleyen bir IoT sistemidir. Ölçüm sonuçları 16x2 I2C LCD ekranda Round Robin yöntemiyle sırayla gösterilmekte; RGB LED ise ışık ve sıcaklık durumuna göre renk değiştirerek görsel geri bildirim sağlamaktadır. Necmettin Erbakan Üniversitesi Bilgisayar Mühendisliği Bölümü **Nesnelerin İnterneti** dersi kapsamında geliştirilmiştir.

🔗 **Wokwi Simülasyon Linki:** [https://wokwi.com/projects/460947442239665153](https://wokwi.com/projects/460947442239665153)

---

## 🧰 Kullanılan Bileşenler

| # | Bileşen | Açıklama |
|---|---------|----------|
| 1 | ESP32 DevKit v1 | Ana mikrodenetleyici |
| 2 | DHT22 | Sıcaklık ve nem sensörü |
| 3 | LDR (wokwi-photoresistor-sensor) | Ortam ışık yoğunluğu sensörü |
| 4 | 16x2 I2C LCD (adres: 0x27) | Veri görüntüleme ekranı |
| 5 | RGB LED (ortak katot) | Görsel durum göstergesi |
| 6 | 220 Ω Direnç × 3 | RGB LED her kanalı için |
| 7 | Push-button | LCD görüntüleme süresi kontrolü |

---

## 📚 Kullanılan Kütüphaneler

| Kütüphane | Amaç |
|-----------|------|
| `DHT.h` | DHT22 sensöründen sıcaklık ve nem okuma |
| `Wire.h` | I2C haberleşme protokolü |
| `LiquidCrystal_I2C.h` | I2C LCD ekran kontrolü |

---

## 🔌 Bağlantı Şeması (Pin Tablosu)

| Bileşen | Bileşen Pini | ESP32 Pini |
|---------|-------------|------------|
| DHT22 | DATA | GPIO 15 |
| DHT22 | VCC / GND | 3.3V / GND |
| LDR | AO (Analog Out) | GPIO 34 |
| LDR | VCC / GND | 3.3V / GND |
| I2C LCD | SDA | GPIO 21 |
| I2C LCD | SCL | GPIO 22 |
| RGB LED – R | 220Ω ile | GPIO 25 |
| RGB LED – G | 220Ω ile | GPIO 26 |
| RGB LED – B | 220Ω ile | GPIO 27 |
| RGB LED – GND | Katot | GND |
| Push-button | OUT | GPIO 18 (pull-up) |

---

## ⚙️ Çalışma Mantığı

### 📊 Sensör Ölçümleri
- **DHT22** → Sıcaklık (°C) ve nem (%) ölçümü
- **LDR** → Ortam ışık yoğunluğu (lux) ölçümü
- Her **3 saniyede** bir seri monitöre aşağıdaki formatta çıktı verilir:
  ```
  Nem: 49.00% || Sıcaklık: 24.00°C || Işık: 244 lux
  ```

### 🖥️ LCD Ekran – Round Robin Gösterimi
Veriler üç ekran arasında sırayla gösterilir:

| Ekran | Satır 1 | Satır 2 |
|-------|---------|---------|
| 1 | `NEM          2` | `49.00 - LOW` |
| 2 | `SICAKLIK     2` | `24.00 - NORMAL` |
| 3 | `ISIK         2` | `AYDINLIK` |

> Sağ üst köşedeki rakam aktif LCD süresini (saniye) gösterir.

### 🔘 Buton İşlevi
Butona her basıldığında LCD güncelleme süresi **2 sn ↔ 4 sn** arasında geçiş yapar. Değişim seri monitöre de yansıtılır:
```
LCD suresi degistirildi: 4 sn
```

### 💡 RGB LED Renk Mantığı

| Işık Durumu | Sıcaklık Durumu | LED Rengi |
|------------|----------------|-----------|
| KARANLIK (< 10 lux) | — | 🔵 Mavi (0, 0, 255) |
| AYDINLIK (≥ 200 lux) | VERY LOW (< 0°C) | 🩵 Açık Mavi (0, 150, 255) |
| AYDINLIK (≥ 200 lux) | NORMAL (0–30°C) | 🟢 Yeşil (0, 255, 0) |
| AYDINLIK (≥ 200 lux) | HIGH (30–40°C) | 🟠 Turuncu (255, 165, 0) |
| AYDINLIK (≥ 200 lux) | VERY HIGH (> 40°C) | 🔴 Kırmızı (255, 0, 0) |

---

## ▶️ Projeyi Çalıştırma Adımları

1. [Wokwi proje linkine](https://wokwi.com/projects/460947442239665153) gidin.
2. **▶ Start Simulation** butonuna tıklayarak simülasyonu başlatın.
3. DHT22 sensörü üzerinden sıcaklık ve nem değerlerini değiştirerek sistemi test edin.
4. LDR sensörünü ayarlayarak ışık yoğunluğunu değiştirin ve LED renginin değişimini gözlemleyin.
5. Push-button'a tıklayarak LCD süresini 2 sn ↔ 4 sn arasında değiştirin.
6. Serial Monitor üzerinden ölçüm çıktılarını takip edin.

> **Not:** Projeyi yerel olarak çalıştırmak istiyorsanız `sketch.ino`, `diagram.json` ve `libraries.txt` dosyalarını Wokwi'ye yükleyebilir ya da Arduino IDE üzerinden gerçek donanıma aktarabilirsiniz.

---

## 📁 Dosya Yapısı

```
├── sketch.ino        # Ana Arduino kodu
├── diagram.json      # Wokwi devre şeması
├── libraries.txt     # Kullanılan kütüphane listesi
└── README.md         # Proje açıklaması
```

---

## 📞 İletişim

- 📧 **Email:** [ulkehaticekubra@gmail.com]
- 👤 **Author:** [Hatice Kübra ÜLKE](https://github.com/Hkubrau)
- 🐛 **Issues:** Feel free to report bugs or suggest features on [GitHub Issues](../../issues)

---

⭐ *Star this repo if you find it helpful!*

*Made with ❤️ by Hatice Kübra ÜLKE*
