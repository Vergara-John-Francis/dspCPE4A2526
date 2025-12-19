<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HydroSync AI - Flood Monitoring System</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        :root {
            --primary: #00f3ff;
            --secondary: #0080ff;
            --danger: #ff4757;
            --warning: #ffa502;
            --safe: #2ed573;
            --dark-bg: #0a0e17;
            --card-bg: rgba(16, 24, 39, 0.8);
            --glass: rgba(255, 255, 255, 0.05);
        }
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', 'Arial', sans-serif;
        }
        
        body {
            background: var(--dark-bg);
            color: white;
            min-height: 100vh;
            overflow-x: hidden;
            background-image: 
                radial-gradient(circle at 10% 20%, rgba(0, 131, 255, 0.1) 0%, transparent 20%),
                radial-gradient(circle at 90% 80%, rgba(0, 243, 255, 0.1) 0%, transparent 20%);
        }
        
        .dashboard {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            grid-template-rows: auto auto auto;
            gap: 20px;
            padding: 20px;
            max-width: 1800px;
            margin: 0 auto;
        }
        
        .header {
            grid-column: 1 / -1;
            background: var(--card-bg);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 25px;
            border: 1px solid rgba(0, 243, 255, 0.1);
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
        }
        
        .logo {
            display: flex;
            align-items: center;
            gap: 15px;
        }
        
        .logo i {
            font-size: 2.5em;
            color: var(--primary);
            text-shadow: 0 0 15px var(--primary);
        }
        
        .logo h1 {
            font-size: 2em;
            background: linear-gradient(90deg, var(--primary), var(--secondary));
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
            font-weight: 700;
        }
        
        .status-indicator {
            display: flex;
            align-items: center;
            gap: 10px;
            padding: 10px 20px;
            border-radius: 50px;
            background: rgba(46, 213, 115, 0.1);
            border: 1px solid var(--safe);
        }
        
        .status-dot {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            background: var(--safe);
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .card {
            background: var(--card-bg);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            padding: 25px;
            border: 1px solid rgba(255, 255, 255, 0.05);
            box-shadow: 0 8px 25px rgba(0, 0, 0, 0.2);
            transition: transform 0.3s, box-shadow 0.3s;
        }
        
        .card:hover {
            transform: translateY(-5px);
            box-shadow: 0 15px 35px rgba(0, 0, 0, 0.3);
        }
        
        .card-title {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
        }
        
        .card-title h2 {
            font-size: 1.3em;
            color: #a0aec0;
            font-weight: 500;
        }
        
        .card-title i {
            font-size: 1.5em;
            color: var(--primary);
        }
        
        .water-level-container {
            grid-column: 1 / 3;
            grid-row: 2;
        }
        
        .water-level {
            height: 300px;
            position: relative;
            background: rgba(0, 0, 0, 0.2);
            border-radius: 15px;
            overflow: hidden;
            margin-top: 10px;
        }
        
        .water-fill {
            position: absolute;
            bottom: 0;
            left: 0;
            right: 0;
            background: linear-gradient(to top, var(--secondary), var(--primary));
            transition: height 2s ease;
        }
        
        .water-markers {
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            padding: 15px;
        }
        
        .marker {
            display: flex;
            justify-content: space-between;
            color: rgba(255, 255, 255, 0.6);
            font-size: 0.9em;
        }
        
        .risk-meter {
            text-align: center;
            padding: 30px 0;
        }
        
        .risk-score {
            font-size: 5em;
            font-weight: 800;
            margin: 20px 0;
            text-shadow: 0 0 20px currentColor;
        }
        
        .risk-low { color: var(--safe); }
        .risk-medium { color: var(--warning); }
        .risk-high { color: var(--danger); }
        .risk-critical { 
            color: var(--danger);
            animation: danger-pulse 1s infinite;
        }
        
        @keyframes danger-pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.7; }
        }
        
        .risk-level {
            font-size: 1.8em;
            margin-bottom: 20px;
            padding: 10px 30px;
            border-radius: 50px;
            display: inline-block;
        }
        
        .evacuation-alert {
            grid-column: 1 / -1;
            background: linear-gradient(90deg, rgba(255, 71, 87, 0.2), rgba(255, 71, 87, 0.05));
            border: 2px solid var(--danger);
            animation: alert-pulse 2s infinite;
        }
        
        @keyframes alert-pulse {
            0%, 100% { border-color: var(--danger); }
            50% { border-color: transparent; }
        }
        
        .alert-content {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        
        .alert-message h3 {
            color: var(--danger);
            font-size: 1.8em;
            margin-bottom: 10px;
        }
        
        .alert-button {
            background: var(--danger);
            color: white;
            border: none;
            padding: 15px 30px;
            border-radius: 10px;
            font-size: 1.1em;
            cursor: pointer;
            transition: all 0.3s;
        }
        
        .alert-button:hover {
            background: #ff2e43;
            transform: scale(1.05);
        }
        
        .ai-analysis {
            grid-column: 3;
            grid-row: 2 / 4;
        }
        
        .ai-response {
            background: rgba(0, 0, 0, 0.2);
            padding: 20px;
            border-radius: 15px;
            margin-top: 15px;
            font-size: 0.95em;
            line-height: 1.6;
        }
        
        .data-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 15px;
            margin-top: 20px;
        }
        
        .data-item {
            background: rgba(0, 0, 0, 0.2);
            padding: 15px;
            border-radius: 10px;
            text-align: center;
        }
        
        .data-value {
            font-size: 2em;
            font-weight: 700;
            margin: 10px 0;
            color: var(--primary);
        }
        
        .data-label {
            font-size: 0.9em;
            color: #a0aec0;
        }
        
        .chart-container {
            height: 200px;
            margin-top: 20px;
        }
        
        .history-container {
            grid-column: 1 / 3;
        }
        
        @media (max-width: 1200px) {
            .dashboard {
                grid-template-columns: 1fr;
            }
            
            .water-level-container,
            .history-container,
            .ai-analysis {
                grid-column: 1;
            }
            
            .ai-analysis {
                grid-row: auto;
            }
        }
    </style>
</head>
<body>
    <div class="dashboard">
        <!-- Header -->
        <div class="header">
            <div class="logo">
                <i class="fas fa-water"></i>
                <h1>HydroSync AI</h1>
                <span>Flood Monitoring System v2.0</span>
            </div>
            <div class="status-indicator">
                <div class="status-dot"></div>
                <span>SYSTEM ACTIVE • LIVE MONITORING</span>
            </div>
        </div>
        
        <!-- Water Level Card -->
        <div class="card water-level-container">
            <div class="card-title">
                <h2><i class="fas fa-wave-square"></i> WATER LEVEL MONITOR</h2>
                <div class="data-item" style="background: transparent; padding: 0;">
                    <div class="data-value" id="currentLevel">0 cm</div>
                    <div class="data-label">CURRENT LEVEL</div>
                </div>
            </div>
            <div class="water-level">
                <div class="water-fill" id="waterFill" style="height: 0%;"></div>
                <div class="water-markers">
                    <div class="marker"><span>200 cm</span><span>EMPTY</span></div>
                    <div class="marker"><span>150 cm</span></div>
                    <div class="marker"><span>100 cm</span><span>SAFE ZONE</span></div>
                    <div class="marker"><span>50 cm</span></div>
                    <div class="marker"><span>0 cm</span><span>FULL CAPACITY</span></div>
                </div>
            </div>
        </div>
        
        <!-- Risk Assessment Card -->
        <div class="card">
            <div class="card-title">
                <h2><i class="fas fa-exclamation-triangle"></i> RISK ASSESSMENT</h2>
                <i class="fas fa-brain"></i>
            </div>
            <div class="risk-meter">
                <div class="risk-level" id="riskLevel">LOW RISK</div>
                <div class="risk-score risk-low" id="riskScore">0</div>
                <div class="data-label">AI-POWERED RISK SCORE</div>
            </div>
            <div class="data-grid">
                <div class="data-item">
                    <div class="data-value" id="percentage">0%</div>
                    <div class="data-label">CAPACITY</div>
                </div>
                <div class="data-item">
                    <div class="data-value" id="distance">0 cm</div>
                    <div class="data-label>SENSOR DISTANCE</div>
                </div>
                <div class="data-item">
                    <div class="data-value" id="trend">STABLE</div>
                    <div class="data-label>TREND</div>
                </div>
            </div>
        </div>
        
        <!-- Evacuation Alert Card (Hidden by default) -->
        <div class="card evacuation-alert" id="evacuationAlert" style="display: none;">
            <div class="alert-content">
                <div class="alert-message">
                    <h3><i class="fas fa-siren-on"></i> EVACUATION REQUIRED</h3>
                    <p>AI analysis indicates critical flood risk. Immediate evacuation recommended.</p>
                </div>
                <button class="alert-button" onclick="acknowledgeEvacuation()">
                    <i class="fas fa-bullhorn"></i> ACKNOWLEDGE ALERT
                </button>
            </div>
        </div>
        
        <!-- AI Analysis Card -->
        <div class="card ai-analysis">
            <div class="card-title">
                <h2><i class="fas fa-robot"></i> AI ANALYSIS</h2>
                <i class="fas fa-microchip"></i>
            </div>
            <div class="ai-response" id="aiAnalysis">
                <p><i class="fas fa-sync fa-spin"></i> Waiting for AI analysis...</p>
            </div>
            <div class="chart-container">
                <canvas id="riskChart"></canvas>
            </div>
        </div>
        
        <!-- History & Statistics Card -->
        <div class="card history-container">
            <div class="card-title">
                <h2><i class="fas fa-history"></i> HISTORY & ANALYTICS</h2>
                <i class="fas fa-chart-line"></i>
            </div>
            <div class="data-grid">
                <div class="data-item">
                    <div class="data-value" id="avgLevel">0 cm</div>
                    <div class="data-label>24H AVG</div>
                </div>
                <div class="data-item">
                    <div class="data-value" id="maxLevel">0 cm</div>
                    <div class="data-label>PEAK LEVEL</div>
                </div>
                <div class="data-item">
                    <div class="data-value" id="readings">0</div>
                    <div class="data-label>TOTAL READINGS</div>
                </div>
            </div>
            <div class="chart-container">
                <canvas id="historyChart"></canvas>
            </div>
        </div>
    </div>
    
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>
        // Initialize charts
        const riskCtx = document.getElementById('riskChart').getContext('2d');
        const historyCtx = document.getElementById('historyChart').getContext('2d');
        
        const riskChart = new Chart(riskCtx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Risk Score',
                    data: [],
                    borderColor: '#00f3ff',
                    backgroundColor: 'rgba(0, 243, 255, 0.1)',
                    tension: 0.4,
                    fill: true
                }]
            },
            options: {
                responsive: true,
                plugins: { legend: { display: false } },
                scales: {
                    y: { max: 100, min: 0, grid: { color: 'rgba(255,255,255,0.1)' } },
                    x: { grid: { display: false } }
                }
            }
        });
        
        const historyChart = new Chart(historyCtx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Water Level',
                    data: [],
                    borderColor: '#0080ff',
                    backgroundColor: 'rgba(0, 128, 255, 0.1)',
                    tension: 0.4,
                    fill: true
                }]
            },
            options: {
                responsive: true,
                plugins: { legend: { display: false } },
                scales: {
                    y: { grid: { color: 'rgba(255,255,255,0.1)' } },
                    x: { grid: { display: false } }
                }
            }
        });
        
        // Update dashboard data
        async function updateDashboard() {
            try {
                const response = await fetch('http://localhost:5000/api/dashboard-data');
                const data = await response.json();
                
                if (data.current) {
                    // Update water level
                    const level = data.current.water_level_cm;
                    const percentage = data.current.water_percentage;
                    
                    document.getElementById('currentLevel').textContent = `${level.toFixed(1)} cm`;
                    document.getElementById('percentage').textContent = `${percentage.toFixed(1)}%`;
                    document.getElementById('distance').textContent = `${data.current.distance_cm.toFixed(1)} cm`;
                    
                    // Update water fill animation
                    document.getElementById('waterFill').style.height = `${percentage}%`;
                    
                    // Update risk assessment
                    if (data.risk_assessment) {
                        const riskScore = data.risk_assessment.risk_score;
                        const riskLevel = data.risk_assessment.risk_level;
                        
                        document.getElementById('riskScore').textContent = Math.round(riskScore);
                        document.getElementById('riskLevel').textContent = `${riskLevel} RISK`;
                        
                        // Update risk color
                        const riskElement = document.getElementById('riskScore');
                        riskElement.className = 'risk-score ';
                        if (riskScore > 80) riskElement.classList.add('risk-critical');
                        else if (riskScore > 60) riskElement.classList.add('risk-high');
                        else if (riskScore > 40) riskElement.classList.add('risk-medium');
                        else riskElement.classList.add('risk-low');
                        
                        // Update AI analysis
                        if (data.risk_assessment.ai_analysis) {
                            const ai = data.risk_assessment.ai_analysis;
                            document.getElementById('aiAnalysis').innerHTML = `
                                <p><strong>${ai.risk_level} RISK DETECTED</strong></p>
                                <p>${ai.analysis}</p>
                                <p><strong>Recommendations:</strong></p>
                                <ul>
                                    ${ai.recommendations ? ai.recommendations.map(r => `<li>${r}</li>`).join('') : '<li>No specific recommendations</li>'}
                                </ul>
                            `;
                        }
                    }
                    
                    // Update statistics
                    if (data.statistics) {
                        document.getElementById('avgLevel').textContent = `${data.statistics.avg_level.toFixed(1)} cm`;
                        document.getElementById('maxLevel').textContent = `${data.statistics.max_level.toFixed(1)} cm`;
                        document.getElementById('trend').textContent = data.statistics.trend.toUpperCase();
                    }
                    
                    document.getElementById('readings').textContent = data.history ? data.history.length : 0;
                    
                    // Update evacuation alert
                    const evacuationAlert = document.getElementById('evacuationAlert');
                    if (data.evacuation_status) {
                        evacuationAlert.style.display = 'block';
                    } else {
                        evacuationAlert.style.display = 'none';
                    }
                    
                    // Update charts
                    if (data.risk_history && data.history) {
                        const timestamps = data.history.slice(-10).map(d => 
                            new Date(d.timestamp).toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'})
                        );
                        const levels = data.history.slice(-10).map(d => d.water_level_cm);
                        const risks = data.risk_history.slice(-10).map(d => d.risk_score);
                        
                        riskChart.data.labels = timestamps;
                        riskChart.data.datasets[0].data = risks;
                        riskChart.update();
                        
                        historyChart.data.labels = timestamps;
                        historyChart.data.datasets[0].data = levels;
                        historyChart.update();
                    }
                }
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        }
        
        // Acknowledge evacuation alert
        async function acknowledgeEvacuation() {
            await fetch('/api/trigger-evacuation', { method: 'POST' });
            document.getElementById('evacuationAlert').style.display = 'none';
        }
        
        // Update dashboard every 3 seconds
        setInterval(updateDashboard, 3000);
        
        // Initial load
        updateDashboard();
        
        // Add some visual effects
        document.addEventListener('DOMContentLoaded', () => {
            // Animate water fill on load
            setTimeout(() => {
                document.getElementById('waterFill').style.transition = 'height 2s ease';
            }, 500);
        });
    </script>
</body>
</html>