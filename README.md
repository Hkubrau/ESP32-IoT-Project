# 🌡️ ESP32 Environmental Monitoring System

> **Necmettin Erbakan University** | Computer Engineering Department | Internet of Things Course — Assignment I

[![Wokwi Simulation](https://img.shields.io/badge/Wokwi-Live%20Simulation-00C300?style=for-the-badge)](https://wokwi.com/projects/460947442239665153)
[![Platform](https://img.shields.io/badge/Platform-ESP32%20DevKit%20v1-E7352C?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Language](https://img.shields.io/badge/Language-Arduino%20C++-00599C?style=for-the-badge&logo=cplusplus)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-Academic-yellow?style=for-the-badge)](#)

---

## 📋 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [Wiring](#-wiring)
- [How It Works](#-how-it-works)
- [How to Run](#️-how-to-run)
- [File Structure](#-file-structure)
- [Libraries](#-libraries)
- [Contact](#-contact)

---

## 🔍 Overview

This project is a real-time **IoT environmental monitoring system** running on an **ESP32 DevKit v1**. It continuously reads temperature, humidity, and ambient light intensity from onboard sensors, cycles through the readings on a 16×2 I²C LCD display using a **Round Robin** scheduling approach, and provides instant visual feedback through an RGB LED whose color reflects the current environmental state. Display timing is user-adjustable via a push-button.

Developed as part of the **Internet of Things** course at Necmettin Erbakan University, Computer Engineering Department.

🔗 **Live Simulation:** [wokwi.com/projects/460947442239665153](https://wokwi.com/projects/460947442239665153)

---

## 🏗 System Architecture

```
┌─────────────────────────────────────────────────────┐
│                    ESP32 DevKit v1                  │
│                                                     │
│  ┌──────────┐   ┌──────────┐   ┌─────────────────┐ │
│  │  DHT22   │   │   LDR    │   │   Push Button   │ │
│  │ GPIO 15  │   │ GPIO 34  │   │    GPIO 18      │ │
│  └────┬─────┘   └────┬─────┘   └────────┬────────┘ │
│       │              │                  │           │
│       └──────────────┼──────────────────┘           │
│              Main Loop (3s period)                  │
│                       │                             │
│          ┌────────────┴────────────┐                │
│          ▼                         ▼                │
│  ┌───────────────┐      ┌────────────────────┐      │
│  │  16×2 I²C LCD │      │      RGB LED       │      │
│  │  GPIO 21/22   │      │  GPIO 25 / 26 / 27 │      │
│  │  Round Robin  │      │   Color = f(T, L)  │      │
│  │  2s ↔ 4s btn  │      │                    │      │
│  └───────────────┘      └────────────────────┘      │
└─────────────────────────────────────────────────────┘
```

**Data Flow:**
```
Sensors ──► ESP32 ──► Serial Monitor   (every 3 seconds)
                  │
                  ├──► LCD Display     (Round Robin: Humidity → Temperature → Light)
                  │
                  └──► RGB LED         (color driven by light + temperature state)
```

---

## 🔌 Hardware Components

| # | Component | Description |
|---|-----------|-------------|
| 1 | **ESP32 DevKit v1** | Main microcontroller |
| 2 | **DHT22** | Temperature & humidity sensor |
| 3 | **LDR** (photoresistor) | Ambient light intensity sensor |
| 4 | **16×2 I²C LCD** (addr: 0x27) | Real-time data display |
| 5 | **RGB LED** (common cathode) | Visual status indicator |
| 6 | **220 Ω Resistor × 3** | Current limiting for each RGB channel |
| 7 | **Push-button** | Display timing toggle (2s ↔ 4s) |

---

## 🔧 Wiring

| Component | Component Pin | ESP32 GPIO |
|-----------|--------------|-----------|
| DHT22 | DATA | **15** |
| DHT22 | VCC / GND | 3.3V / GND |
| LDR | AO (Analog Out) | **34** |
| LDR | VCC / GND | 3.3V / GND |
| I²C LCD | SDA | **21** |
| I²C LCD | SCL | **22** |
| RGB LED – R | via 220Ω | **25** |
| RGB LED – G | via 220Ω | **26** |
| RGB LED – B | via 220Ω | **27** |
| RGB LED – GND | Cathode | GND |
| Push-button | OUT | **18** (pull-up) |

> ℹ️ The I²C LCD shares the SDA/SCL bus on GPIO 21 and 22.

---

## ⚙️ How It Works

### 📊 Sensor Readings

Every **3 seconds**, all sensor values are printed to the Serial Monitor in the following format:

```
Humidity: 49.00% || Temperature: 24.00°C || Light: 244 lux
```

### 🖥️ LCD — Round Robin Display

Three screens cycle in sequence, each shown for the configured duration:

| Screen | Line 1 | Line 2 |
|--------|--------|--------|
| 1 | `HUMIDITY     2` | `49.00 - LOW` |
| 2 | `TEMPERATURE  2` | `24.00 - NORMAL` |
| 3 | `LIGHT        2` | `BRIGHT` |

> The number in the top-right corner indicates the current display duration in seconds.

### 🔘 Push-Button

Each button press toggles the LCD update interval between **2 seconds** and **4 seconds**. The change is also printed to the Serial Monitor:

```
Display interval changed: 4s
```

### 💡 RGB LED Color Logic

| Light Condition | Temperature Condition | LED Color |
|----------------|----------------------|-----------|
| 🌑 DARK `< 10 lux` | — | 🔵 Blue `(0, 0, 255)` |
| ☀️ BRIGHT `≥ 200 lux` | VERY LOW `< 0°C` | 🩵 Light Blue `(0, 150, 255)` |
| ☀️ BRIGHT `≥ 200 lux` | NORMAL `0–30°C` | 🟢 Green `(0, 255, 0)` |
| ☀️ BRIGHT `≥ 200 lux` | HIGH `30–40°C` | 🟠 Orange `(255, 165, 0)` |
| ☀️ BRIGHT `≥ 200 lux` | VERY HIGH `> 40°C` | 🔴 Red `(255, 0, 0)` |

---

## ▶️ How to Run

### Option 1 — Wokwi Simulation (Recommended)

1. Open the **[live simulation](https://wokwi.com/projects/460947442239665153)** → click ▶ **Start Simulation**
2. Adjust the **DHT22 slider** to test different temperature and humidity values
3. Adjust the **LDR slider** to change ambient light and observe RGB LED color changes
4. Click the **push-button** to toggle between 2s and 4s display intervals
5. Open **Serial Monitor** to follow raw sensor output

### Option 2 — Physical Hardware

1. **Install the required libraries** via Arduino Library Manager:
   ```
   DHT sensor library     → Adafruit
   LiquidCrystal_I2C      → Frank de Brabander
   ```

2. Wire all components according to the pin table above

3. Upload `sketch.ino` to an ESP32 using Arduino IDE

4. Open the **Serial Monitor** at **115200 baud**

---

## 📁 File Structure

```
ESP32-IoT-Project/
├── sketch.ino                             # Main application code
├── diagram.json                           # Wokwi circuit definition
├── libraries.txt                          # Required library list
├── wokwi-project.txt                      # Wokwi project reference
├── HaticeKubraUlke_IoT_Odev1_Rapor.docx  # Project report (Turkish)
└── README.md                              # This file
```

---

## 📚 Libraries

| Library | Purpose |
|---------|---------|
| `DHT.h` (Adafruit) | DHT22 temperature & humidity reading |
| `Wire.h` | I²C communication protocol |
| `LiquidCrystal_I2C.h` | 16×2 LCD driver |

---

## 👤 Contact

**Hatice Kübra ÜLKE**

[![GitHub](https://img.shields.io/badge/GitHub-Hkubrau-181717?style=flat-square&logo=github)](https://github.com/Hkubrau)
[![Email](https://img.shields.io/badge/Email-ulkehaticekubra%40gmail.com-D14836?style=flat-square&logo=gmail&logoColor=white)](mailto:ulkehaticekubra@gmail.com)

> 🐛 Feel free to report bugs or suggest features via [GitHub Issues](../../issues).

---

<div align="center">

⭐ *Star this repo if you find it helpful!*

*Made with ❤️ by Hatice Kübra ÜLKE*

*Necmettin Erbakan University — Computer Engineering — IoT Course*

</div>
