var ws;
var ledState = false;

function init() {
    updateUI();
    
    const espIp = prompt(
        "Enter your Ngrok hostname (e.g., xyz.ngrok-free.app or xyz.tcp.ngrok.io):",
        "f39f-116-75-223-202.ngrok-free.app"
    ) || "f39f-116-75-223-202.ngrok-free.app";

    const ngrokUrl = espIp.replace(/^(https?:\/\/|wss?:\/\/)/, "").trim(); // Clean input

    // Basic validation: check if it's a ngrok domain
    if (
        !ngrokUrl.includes("ngrok.io") && 
        !ngrokUrl.includes("ngrok-free.app") &&
        !ngrokUrl.includes("tcp.ngrok.io")
    ) {
        alert("Invalid Ngrok URL! Please try again.");
        return;
    }

    // Decide if wss:// or ws:// based on your server (secure or not)
    let protocol = location.protocol === 'https:' ? 'wss' : 'ws';

    const wsUrl = `${protocol}://${ngrokUrl}`;
    connectWebSocket(wsUrl);
}

function connectWebSocket(url) {
    ws = new WebSocket(url);

    ws.onopen = function() {
        console.log('Connected to WebSocket server');
        updateConnectionStatus('Connected', 'var(--success-color)', `Connected to ESP32 via ${url}`, '#e8f5e9');
        document.getElementById('toggleBtn').disabled = false;
    };
    
    ws.onmessage = function(event) {
        console.log('Received:', event.data);
        if (event.data === 'on' || event.data === 'off') {
            ledState = (event.data === 'on');
            updateUI();
        }
    };
    
    ws.onerror = function(error) {
        console.error('WebSocket Error:', error);
        updateConnectionStatus('Error', 'var(--error-color)', 'WebSocket connection error.', '#ffebee');
        document.getElementById('toggleBtn').disabled = true;
    };
    
    ws.onclose = function() {
        console.log('WebSocket closed');
        updateConnectionStatus('Disconnected', '#cccccc', 'Connection closed. Refresh to retry.', '#f8f9fa');
        document.getElementById('toggleBtn').disabled = true;
    };
}

function updateConnectionStatus(status, color, info, bgColor) {
    const statusEl = document.getElementById('connectionStatus');
    const dotEl = document.getElementById('connectionDot');
    const infoEl = document.getElementById('connectionInfo');
    
    if (statusEl) statusEl.textContent = status;
    if (dotEl) dotEl.style.backgroundColor = color;
    if (infoEl) {
        infoEl.textContent = info;
        infoEl.style.backgroundColor = bgColor;
    }
}

function updateUI() {
    const ledVisual = document.getElementById('ledVisual');
    if (ledVisual) {
        ledVisual.classList.toggle('led-on', ledState);
    }
}

function toggleLED() {
    if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send('toggle');
        ledState = !ledState;
        updateUI();
    } else {
        console.error('WebSocket not ready');
        updateConnectionStatus('Error', 'var(--error-color)', 'WebSocket not connected.', '#ffebee');
    }
}

// Initialize safely
if (document.readyState === 'complete') {
    init();
} else {
    window.addEventListener('DOMContentLoaded', init);
}
