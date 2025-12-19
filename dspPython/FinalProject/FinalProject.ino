# backend_server.py
from flask import Flask, request, jsonify
from flask_cors import CORS
from datetime import datetime
import requests
import json
import threading
import time
import numpy as np
from sklearn.ensemble import IsolationForest
import joblib
import os

app = Flask(__name__)
CORS(app)

# LM Studio API Configuration
LM_STUDIO_URL = "http://localhost:1234/v1/completions"
LM_MODEL = "qwen2-1.5b-instruct"  # Your loaded model in LM Studio

# In-memory storage for sensor data
sensor_data = []
risk_history = []
evacuation_status = False

# Risk Assessment Model
class RiskAssessmentModel:
    def __init__(self):
        self.model = IsolationForest(contamination=0.1, random_state=42)
        self.is_trained = False
        self.risk_factors = []
        
    def train_model(self, historical_data):
        """Train anomaly detection model"""
        if len(historical_data) > 10:
            data_array = np.array(historical_data).reshape(-1, 1)
            self.model.fit(data_array)
            self.is_trained = True
    
    def calculate_risk(self, current_level, historical_data):
        """Calculate flood risk score (0-100)"""
        if not historical_data:
            return 50  # Default medium risk
        
        # Multiple risk factors
        current_risk = 0
        
        # 1. Current level risk (50% weight)
        level_percentage = (current_level / 200) * 100
        level_risk = min(100, max(0, (level_percentage - 50) * 2))
        current_risk += level_risk * 0.5
        
        # 2. Rate of change risk (30% weight)
        if len(historical_data) > 5:
            recent_changes = np.diff(historical_data[-5:])
            avg_change = np.mean(recent_changes)
            if avg_change > 5:  # Rapid rising
                change_risk = min(100, avg_change * 10)
                current_risk += change_risk * 0.3
        
        # 3. Anomaly detection risk (20% weight)
        if self.is_trained and len(historical_data) > 0:
            anomaly_score = self.model.score_samples([[current_level]])[0]
            anomaly_risk = max(0, min(100, (1 - anomaly_score) * 100))
            current_risk += anomaly_risk * 0.2
        
        return min(100, current_risk)

risk_model = RiskAssessmentModel()

def analyze_with_ai(water_data, risk_score):
    """Use LM Studio for intelligent analysis"""
    prompt = f"""
    Analyze flood risk scenario:
    
    Current Water Level: {water_data['water_level_cm']} cm
    Tank Height: {water_data['tank_height_cm']} cm
    Fill Percentage: {water_data['water_percentage']}%
    Risk Score: {risk_score}/100
    Historical Trend: {'Rising' if len(sensor_data) > 2 and sensor_data[-1]['water_level_cm'] > sensor_data[-2]['water_level_cm'] else 'Stable'}
    
    Provide analysis with:
    1. Risk Level (Low/Medium/High/Critical)
    2. Short analysis
    3. Recommended actions
    4. Evacuation recommendation (Yes/No)
    
    Format response as JSON with keys: risk_level, analysis, recommendations, evacuation_needed
    """
    
    try:
        response = requests.post(
            LM_STUDIO_URL,
            json={
                "model": LM_MODEL,
                "prompt": prompt,
                "max_tokens": 500,
                "temperature": 0.7
            },
            timeout=10
        )
        
        if response.status_code == 200:
            result = response.json()
            ai_response = result['choices'][0]['text']
            
            # Parse JSON from AI response
            try:
                return json.loads(ai_response)
            except:
                # Fallback if AI doesn't return valid JSON
                return {
                    "risk_level": "High" if risk_score > 70 else "Medium" if risk_score > 40 else "Low",
                    "analysis": "AI analysis unavailable",
                    "recommendations": ["Monitor water level", "Check local weather alerts"],
                    "evacuation_needed": risk_score > 80
                }
    except:
        pass
    
    return None

@app.route('/api/sensor-data', methods=['POST'])
def receive_sensor_data():
    """Receive data from ESP32"""
    global sensor_data, evacuation_status
    
    data = request.json
    data['timestamp'] = datetime.now().isoformat()
    
    # Store data
    sensor_data.append(data)
    
    # Keep only last 100 readings
    if len(sensor_data) > 100:
        sensor_data = sensor_data[-100:]
    
    # Calculate risk score
    water_levels = [d['water_level_cm'] for d in sensor_data]
    risk_model.train_model(water_levels)
    risk_score = risk_model.calculate_risk(data['water_level_cm'], water_levels)
    
    # Get AI analysis
    ai_analysis = analyze_with_ai(data, risk_score)
    
    # Determine evacuation need
    if ai_analysis:
        evacuation_needed = ai_analysis.get('evacuation_needed', False)
        risk_level = ai_analysis.get('risk_level', 'Unknown')
    else:
        evacuation_needed = risk_score > 80
        risk_level = "Critical" if risk_score > 80 else "High" if risk_score > 60 else "Medium" if risk_score > 40 else "Low"
    
    # Update evacuation status
    if evacuation_needed and not evacuation_status:
        evacuation_status = True
        # Trigger alerts
        trigger_alerts(data, risk_score, risk_level)
    
    # Store risk assessment
    risk_entry = {
        'timestamp': data['timestamp'],
        'risk_score': risk_score,
        'risk_level': risk_level,
        'evacuation_needed': evacuation_needed,
        'water_level': data['water_level_cm'],
        'ai_analysis': ai_analysis
    }
    risk_history.append(risk_entry)
    
    return jsonify({
        'status': 'received',
        'risk_assessment': risk_entry
    })

def trigger_alerts(data, risk_score, risk_level):
    """Trigger emergency alerts"""
    print(f"🚨 EVACUATION ALERT! Risk Level: {risk_level}, Score: {risk_score}")
    # Here you can add:
    # - SMS alerts (Twilio)
    # - Email notifications
    # - IoT device triggers
    # - External API calls

@app.route('/api/dashboard-data', methods=['GET'])
def get_dashboard_data():
    """Get data for web dashboard"""
    if not sensor_data:
        return jsonify({'error': 'No data available'})
    
    latest = sensor_data[-1]
    latest_risk = risk_history[-1] if risk_history else None
    
    return jsonify({
        'current': latest,
        'risk_assessment': latest_risk,
        'history': sensor_data[-24:],  # Last 24 readings
        'risk_history': risk_history[-24:],
        'statistics': {
            'avg_level': np.mean([d['water_level_cm'] for d in sensor_data[-24:]]),
            'max_level': np.max([d['water_level_cm'] for d in sensor_data[-24:]]),
            'trend': 'rising' if len(sensor_data) > 2 and sensor_data[-1]['water_level_cm'] > sensor_data[-2]['water_level_cm'] else 'stable'
        },
        'evacuation_status': evacuation_status,
        'system_status': 'active'
    })

@app.route('/api/trigger-evacuation', methods=['POST'])
def trigger_manual_evacuation():
    """Manually trigger evacuation"""
    global evacuation_status
    evacuation_status = True
    return jsonify({'status': 'evacuation_triggered', 'message': 'Emergency evacuation initiated'})

if __name__ == '__main__':
    print("Starting Flood Monitoring Server...")
    app.run(host='0.0.0.0', port=5000, debug=True)