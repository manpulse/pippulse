# Pulsepoint

ESP32-based vital signs monitor with web dashboard and AI health analysis.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        HARDWARE                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐    │
│  │  ESP32   │  │ MAX30102 │  │ DS18B20  │  │  AD8232  │    │
│  │  DevKit  │  │ SpO2/HR  │  │   Temp   │  │   ECG    │    │
│  └────┬─────┘  └────┬─────┘  └────┬─────┘  └────┬─────┘    │
│       │           I2C          OneWire       Analog        │
│       └────────────┴──────────────┴────────────┘           │
└─────────────────────────┬───────────────────────────────────┘
                          │ WebSocket
┌─────────────────────────▼───────────────────────────────────┐
│                     WEB DASHBOARD                           │
└─────────────────────────┬───────────────────────────────────┘
                          │ API
┌─────────────────────────▼───────────────────────────────────┐
│                     GEMINI AI                               │
└─────────────────────────────────────────────────────────────┘
```

## Project Structure

```
pulsepoint/
├── firmware/
│   ├── 01_temperature/     # DS18B20
│   ├── 02_spo2/            # MAX30102
│   ├── 03_webserver/       # WebSocket
│   └── 04_ecg/             # AD8232
├── web/
│   ├── dashboard/          # UI
│   └── api-proxy/          # Gemini AI
├── WIRING.md               # Wiring diagrams
└── README.md
```

## Timeline

| Week | Focus | Hardware |
|------|-------|----------|
| 1 | Temperature | DS18B20 |
| 2 | SpO2 + Heart Rate | MAX30102 |
| 3 | Web Dashboard | - |
| 4 | AI Doctor Summary | - |
| 5 | ECG (Advanced) | AD8232 |

See [WIRING.md](WIRING.md) for connection diagrams.
