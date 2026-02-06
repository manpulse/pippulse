/**
 * Gemini AI Health Summary Proxy
 * 
 * Receives vital signs data from ESP32 dashboard
 * Returns AI-generated doctor summary
 */

import express from 'express';
import cors from 'cors';
import { GoogleGenerativeAI } from '@google/generative-ai';

const app = express();
app.use(cors());
app.use(express.json());

const genAI = new GoogleGenerativeAI(process.env.GEMINI_API_KEY);
const model = genAI.getGenerativeModel({ model: 'gemini-2.5-flash' });

// Health analysis prompt template
const buildPrompt = (data) => `
You are a professional doctor analyzing vital signs data.
Provide a brief, clear health summary in 3-4 sentences.

Patient Vitals:
- Heart Rate: ${data.heartRate} BPM
- SpO2: ${data.spo2}%
- Temperature: ${data.temperature}°C

Respond with:
1. Overall status (Normal/Attention Needed/Critical)
2. Key observations
3. Simple recommendation
`;
//prompt to be changed later @TODO
// POST /analyze - Main endpoint
app.post('/analyze', async (req, res) => {
    try {
        const { heartRate, spo2, temperature } = req.body;


        if (!heartRate || !spo2 || !temperature) {
            return res.status(400).json({ error: 'Missing vital signs data' });
        }

        const prompt = buildPrompt({ heartRate, spo2, temperature });
        const result = await model.generateContent(prompt);
        const summary = result.response.text();

        res.json({
            summary,
            timestamp: new Date().toISOString(),
            vitals: { heartRate, spo2, temperature }
        });

    } catch (error) {
        console.error('Gemini API error:', error.message);
        res.status(500).json({ error: 'AI analysis failed' });
    }
});

// Health check
app.get('/health', (req, res) => {
    res.json({ status: 'ok' });
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
    console.log(`Health AI Proxy running on port ${PORT}`);
});
