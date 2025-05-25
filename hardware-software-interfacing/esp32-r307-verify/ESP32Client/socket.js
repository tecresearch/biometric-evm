        const espIp=prompt("Enter your ESP32 Websocket:HOST");
        const socket = new WebSocket(`ws://${espIp}:81/`);
        const statusText = document.getElementById('statusText');
        const statusIndicator = document.getElementById('statusIndicator');
        const statusDetail = document.getElementById('statusDetail');
        const scanner = document.getElementById('scanner');
        const scannerMessage = document.getElementById('scannerMessage');
        const enrollBtn = document.getElementById('enrollBtn');
        const matchBtn = document.getElementById('matchBtn');
        const logDiv = document.getElementById('log');
        
        let currentState = 'disconnected';
        let operationInProgress = false;
        
        // WebSocket handlers
        socket.onopen = function(e) {
            addLog('WebSocket connection established');
            updateStatus('connecting', 'Connecting to fingerprint sensor...');
        };
        
        socket.onmessage = function(event) {
            if (event.data.startsWith('state:')) {
                const state = event.data.split(':')[1];
                handleStateChange(state);
            } else {
                addLog('SYSTEM: ' + event.data);
                
                // Update scanner based on messages
                if (event.data.includes('successful')) {
                    scannerSuccess();
                } else if (event.data.includes('failed') || event.data.includes('error')) {
                    scannerError();
                } else if (event.data.includes('Place finger')) {
                    startScanner();
                }
            }
        };
        
        socket.onclose = function(event) {
            if (event.wasClean) {
                addLog(`Connection closed: code=${event.code}, reason=${event.reason}`);
            } else {
                addLog('Connection lost - attempting to reconnect...');
                updateStatus('disconnected', 'Connection to device lost');
                setTimeout(() => window.location.reload(), 3000);
            }
        };
        
        socket.onerror = function(error) {
            addLog('WebSocket error: ' + error.message);
            updateStatus('error', 'Connection error occurred');
        };
        
        // State handling
        function handleStateChange(state) {
            currentState = state;
            
            switch(state) {
                case 'connected':
                    updateStatus('connected', 'System ready');
                    enableButtons();
                    resetScanner();
                    break;
                case 'disconnected':
                    updateStatus('disconnected', 'Fingerprint sensor not detected');
                    disableButtons();
                    break;
                case 'enrolling':
                    updateStatus('processing', 'Enrollment in progress');
                    disableButtons();
                    startScanner();
                    break;
                case 'matching':
                    updateStatus('processing', 'Verification in progress');
                    disableButtons();
                    startScanner();
                    break;
                case 'enroll_success':
                    updateStatus('connected', 'Enrollment successful');
                    enableButtons();
                    scannerSuccess();
                    break;
                case 'match_success':
                    updateStatus('connected', 'Verification successful');
                    enableButtons();
                    scannerSuccess();
                    break;
                default:
                    updateStatus('error', 'Unknown state: ' + state);
            }
        }
        
        function updateStatus(type, message) {
            statusText.textContent = message;
            statusDetail.textContent = getStatusDetail(type);
            
            // Update indicator
            statusIndicator.className = 'status-indicator';
            switch(type) {
                case 'connected':
                    statusIndicator.classList.add('status-connected');
                    break;
                case 'disconnected':
                case 'error':
                    statusIndicator.classList.add('status-disconnected');
                    break;
                case 'connecting':
                case 'processing':
                    statusIndicator.classList.add('status-processing');
                    break;
            }
        }
        
        function getStatusDetail(type) {
            const details = {
                'connected': `System ready for operations. ${finger.templateCount || 0} fingerprints enrolled.`,
                'disconnected': 'Check fingerprint sensor connection and power.',
                'connecting': 'Establishing connection with biometric device...',
                'enrolling': 'Follow instructions to enroll new fingerprint.',
                'matching': 'Place your finger on the scanner for verification.',
                'enroll_success': 'New fingerprint successfully enrolled in system.',
                'match_success': 'Fingerprint verified - access granted.',
                'error': 'An error occurred. Check system logs for details.'
            };
            return details[type] || 'System status unknown.';
        }
        
        // Scanner effects
        function startScanner() {
            scanner.className = 'scanner-placeholder scanner-active';
            scannerMessage.textContent = 'Scanning fingerprint...';
            operationInProgress = true;
        }
        
        function scannerSuccess() {
            scanner.className = 'scanner-placeholder scanner-active scanner-success';
            scannerMessage.textContent = 'Fingerprint recognized!';
            operationInProgress = false;
            setTimeout(resetScanner, 2000);
        }
        
        function scannerError() {
            scanner.className = 'scanner-placeholder scanner-active scanner-error';
            scannerMessage.textContent = 'Scan failed - try again';
            operationInProgress = false;
            setTimeout(resetScanner, 2000);
        }
        
        function resetScanner() {
            if (!operationInProgress) {
                scanner.className = 'scanner-placeholder';
                scannerMessage.textContent = 'Place your finger on the scanner';
            }
        }
        
        // Button controls
        function enableButtons() {
            enrollBtn.disabled = false;
            matchBtn.disabled = false;
        }
        
        function disableButtons() {
            enrollBtn.disabled = true;
            matchBtn.disabled = true;
        }
        
        // Logging
        function addLog(message) {
            const entry = document.createElement('div');
            entry.className = 'log-entry';
            
            const time = document.createElement('span');
            time.className = 'log-time';
            time.textContent = new Date().toLocaleTimeString();
            
            const text = document.createElement('span');
            text.textContent = message;
            
            entry.appendChild(time);
            entry.appendChild(text);
            logDiv.appendChild(entry);
            logDiv.scrollTop = logDiv.scrollHeight;
        }
        
        // Button event listeners
        enrollBtn.addEventListener('click', function() {
            addLog('COMMAND: Initiating fingerprint enrollment');
            socket.send('enroll');
        });
        
        matchBtn.addEventListener('click', function() {
            addLog('COMMAND: Initiating fingerprint verification');
            socket.send('match');
        });
        
        // Simulate fingerprint data (for demo purposes)
        const finger = {
            templateCount: 0
        };
        
        // Update template count when enrollments succeed
        socket.addEventListener('message', function(event) {
            if (event.data.includes('Enrollment successful')) {
                finger.templateCount++;
            }
        });
   