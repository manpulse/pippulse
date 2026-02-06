# Gemini API Proxy

## Setup

```bash
cd web/api-proxy
npm install
```

## Run

```bash
GEMINI_API_KEY=your_key_here npm start
```

## API

**POST /analyze**
```json
{
  "heartRate": 72,
  "spo2": 98,
  "temperature": 36.5
}
```

**Response**
```json
{
  "summary": "Overall status: Normal. Your vital signs are within healthy ranges...",
  "timestamp": "2026-02-06T23:56:07Z",
  "vitals": { "heartRate": 72, "spo2": 98, "temperature": 36.5 }
}
```
