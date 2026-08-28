# Smart Tunnel Management System

An ESP32-based IoT prototype for tunnel environmental monitoring and automatic ventilation control.

## Current firmware implementation

The current ESP32 firmware implements:

- ESP32 as the main controller
- MQ2 analog air-quality/gas sensing
- BH1750 light-intensity sensing
- L298N motor-driver interface
- DC fan speed control using PWM
- Automatic fan control based on MQ2 raw ADC thresholds
- Manual fan control from Blynk
- Wi-Fi connectivity
- Blynk Cloud monitoring
- Pollution and low-light event notifications

> **Important:** This repository's current firmware uses **MQ2**, not MQ135.  
> ESP32-CAM is not controlled by the current `Smart_Tunnel_ESP32.ino` firmware. If a separate camera firmware is added later, place it under `firmware/ESP32_CAM/`.

## Repository structure

```text
Smart-Tunnel-Management-System/
├── README.md
├── .gitignore
├── firmware/
│   └── Smart_Tunnel_ESP32/
│       └── Smart_Tunnel_ESP32.ino
├── hardware/
│   ├── hardware-architecture.png
│   ├── circuit-diagram.png
│   └── pin-configuration.md
├── documentation/
│   ├── User-Manual.pdf
│   └── Installation-Guide.pdf
└── images/
    ├── prototype.jpg
    ├── blynk-dashboard.png
    └── hardware.jpg
```

## System architecture

The operating chain is:

```text
MQ2 Sensor ──┐
             ├──> ESP32 ──> Air-quality decision ──> PWM ──> L298N ──> DC Fan
BH1750 ──────┘
                │
                └── Wi-Fi ──> Blynk Cloud ──> Blynk Dashboard
```

## Automatic fan control

The firmware uses raw MQ2 ADC values:

| MQ2 raw ADC | Fan state | PWM |
|---:|---|---:|
| 0–490 | OFF | 0 |
| 491–900 | LOW | 90 |
| 901–1500 | MEDIUM | 170 |
| >1500 | HIGH | 255 |

These values are **starting calibration thresholds**, not standardized ppm limits. Calibrate them for the actual sensor and tunnel environment before research or field deployment.

## Blynk virtual pins

| Pin | Function | Direction |
|---|---|---|
| V0 | MQ2 air-quality raw ADC | ESP32 → Blynk |
| V1 | Automatic/Manual mode | Blynk → ESP32 |
| V2 | BH1750 light level | ESP32 → Blynk |
| V3 | Current fan PWM | ESP32 → Blynk |
| V4 | Manual fan PWM | Blynk → ESP32 |

### Blynk mode

- `V1 = 1` → Automatic mode
- `V1 = 0` → Manual mode
- `V4 = 0–255` → Manual fan PWM

## Blynk event codes

The current firmware calls:

```text
polution_alert
lightdamage
```

The Blynk event codes must match these strings exactly.

## Hardware pin summary

| Component | Connection |
|---|---|
| MQ2 AO | ESP32 GPIO34 |
| BH1750 SDA | ESP32 default I2C SDA |
| BH1750 SCL | ESP32 default I2C SCL |
| L298N ENA/PWM | ESP32 GPIO15 |
| L298N IN1 | ESP32 GPIO2 |
| L298N IN2 | Not connected in current firmware |
| L298N OUT1/OUT2 | DC fan |
| ESP32/L298N GND | Common ground |

## Required libraries

Install through Arduino IDE Library Manager:

- Blynk
- BH1750

Also install the ESP32 board package by Espressif Systems.

## Quick start

1. Open `firmware/Smart_Tunnel_ESP32/Smart_Tunnel_ESP32.ino`.
2. Replace:
   - `BLYNK_TEMPLATE_ID`
   - `BLYNK_TEMPLATE_NAME`
   - `BLYNK_AUTH_TOKEN`
   - Wi-Fi SSID
   - Wi-Fi password
3. Create Blynk V0–V4 datastreams.
4. Create Blynk events with codes `polution_alert` and `lightdamage`.
5. Wire the hardware according to `hardware/pin-configuration.md`.
6. Select the correct ESP32 board and COM port in Arduino IDE.
7. Upload the firmware.
8. Open Serial Monitor at 115200 baud.
9. Test manual mode first, then automatic mode.
10. Calibrate MQ2 thresholds before final deployment.

## Security

Never commit a real Blynk Auth Token or Wi-Fi password to a public repository. Use placeholders in the source published on GitHub.

## Documentation

See: Smart_Tunnel_IoT_Full_Documentation.docs

## Disclaimer

The MQ2 value in this project is a raw ADC reading. It should not be interpreted as a standardized air-quality index or gas concentration without appropriate calibration and validation.
