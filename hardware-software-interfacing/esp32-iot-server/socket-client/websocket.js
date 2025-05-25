

var ws;
var ledState = false;

function init() {
    updateUI();
    
    // For demo purposes - in a real implementation, replace this with your actual ESP32 IP
    const espIp = prompt("Enter ESP32 IP Address:", "192.168.1.100") || "192.168.1.100";
    
    // Connect to the WebSocket server on the ESP32
    ws = new WebSocket(`ws://${espIp}`);
    
    ws.onopen = function() {
        console.log('Connected to WebSocket server');
        document.getElementById('connectionStatus').textContent = 'Connected';
        document.getElementById('connectionDot').style.backgroundColor = 'var(--success-color)';
        document.getElementById('connectionInfo').textContent = `Connected to ESP32 at ${espIp}`;
        document.getElementById('connectionInfo').style.backgroundColor = '#e8f5e9';
        document.getElementById('toggleBtn').disabled = false;
    };
    
    ws.onmessage = function(event) {
        console.log('Received message: ' + event.data);
        if (event.data === 'on' || event.data === 'off') {
            ledState = event.data === 'on';
            updateUI();
        }
    };
    
    ws.onerror = function(error) {
        console.error('WebSocket Error: ' + error);
        document.getElementById('connectionStatus').textContent = 'Connection Error';
        document.getElementById('connectionDot').style.backgroundColor = 'var(--error-color)';
        document.getElementById('connectionInfo').textContent = 'Failed to connect to ESP32. Please check the IP address and try again.';
        document.getElementById('connectionInfo').style.backgroundColor = '#ffebee';
    };
    
    ws.onclose = function() {
        console.log('WebSocket connection closed');
        document.getElementById('connectionStatus').textContent = 'Disconnected';
        document.getElementById('connectionDot').style.backgroundColor = '#cccccc';
        document.getElementById('connectionInfo').textContent = 'Connection closed. Refresh page to reconnect.';
        document.getElementById('connectionInfo').style.backgroundColor = '#f8f9fa';
        document.getElementById('toggleBtn').disabled = true;
    };
}

function updateUI() {
    const ledVisual = document.getElementById('ledVisual');
    if (ledState) {
        ledVisual.classList.add('led-on');
    } else {
        ledVisual.classList.remove('led-on');
    }
}

function toggleLED() {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send('toggle');
        // Optimistically update the UI
        ledState = !ledState;
        updateUI();
    } else {
        console.log('WebSocket is not open');
    }
}

window.onload = init;
