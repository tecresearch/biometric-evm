#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <WebServer.h>

// --- Access Point Credentials ---
const char* ap_ssid = "ESP32_Setup";    // Change if you want
const char* ap_password = "12345678";   // AP Password

// --- WebServer Object ---
WebServer server(80);

// --- WiFi Credentials (entered by user) ---
String wifiSSID = "";
String wifiPassword = "";

// --- Beautiful HTML Form for WiFi Credentials ---
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 WiFi Setup</title>
  <style>
    :root {
      --gradient-pink: #ff6b6b;
      --gradient-yellow: #feca57;
      --gradient-blue: #48dbfb;
      --gradient-white: #ffffff;
      --gradient-black: #2d3436;
    }
    
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, var(--gradient-pink), var(--gradient-yellow), var(--gradient-blue));
      background-size: 400% 400%;
      animation: gradientBG 15s ease infinite;
      margin: 0;
      padding: 0;
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      color: var(--gradient-black);
    }
    
    @keyframes gradientBG {
      0% { background-position: 0% 50%; }
      50% { background-position: 100% 50%; }
      100% { background-position: 0% 50%; }
    }
    
    .container {
      background-color: rgba(255, 255, 255, 0.9);
      border-radius: 15px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
      padding: 2.5rem;
      width: 90%;
      max-width: 450px;
      text-align: center;
      transition: transform 0.3s ease;
    }
    
    .container:hover {
      transform: translateY(-5px);
    }
    
    h1 {
      color: var(--gradient-black);
      margin-bottom: 1.5rem;
      font-weight: 700;
      position: relative;
    }
    
    h1::after {
      content: '';
      display: block;
      width: 60px;
      height: 4px;
      background: linear-gradient(to right, var(--gradient-pink), var(--gradient-blue));
      margin: 0.5rem auto 0;
      border-radius: 2px;
    }
    
    .form-group {
      margin-bottom: 1.5rem;
      text-align: left;
    }
    
    label {
      display: block;
      margin-bottom: 0.5rem;
      font-weight: 600;
      color: var(--gradient-black);
    }
    
    input {
      width: 100%;
      padding: 0.8rem 1rem;
      border: 2px solid #eee;
      border-radius: 8px;
      font-size: 1rem;
      transition: all 0.3s ease;
      box-sizing: border-box;
    }
    
    input:focus {
      border-color: var(--gradient-blue);
      outline: none;
      box-shadow: 0 0 0 3px rgba(72, 219, 251, 0.3);
    }
    
    input[type="password"] {
      letter-spacing: 1px;
    }
    
    .btn {
      background: linear-gradient(to right, var(--gradient-pink), var(--gradient-blue));
      color: white;
      border: none;
      padding: 0.8rem 2rem;
      font-size: 1rem;
      font-weight: 600;
      border-radius: 8px;
      cursor: pointer;
      transition: all 0.3s ease;
      text-transform: uppercase;
      letter-spacing: 1px;
      width: 100%;
      margin-top: 0.5rem;
    }
    
    .btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
    }
    
    .btn:active {
      transform: translateY(0);
    }
    
    .logo {
      width: 60px;
      height: 60px;
      margin-bottom: 1rem;
      animation: pulse 2s infinite;
    }
    
    @keyframes pulse {
      0% { transform: scale(1); }
      50% { transform: scale(1.1); }
      100% { transform: scale(1); }
    }
    
    .status {
      margin-top: 1.5rem;
      padding: 1rem;
      border-radius: 8px;
      display: none;
    }
    
    .success {
      background-color: rgba(46, 213, 115, 0.2);
      color: #2ed573;
    }
    
    .error {
      background-color: rgba(255, 71, 87, 0.2);
      color: #ff4757;
    }
  </style>
</head>
<body>
  <div class="container">
    <svg class="logo" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
      <path d="M12 2L2 7l10 5 10-5-10-5zM2 17l10 5 10-5M2 12l10 5 10-5"></path>
    </svg>
    <h1>WiFi Configuration</h1>
    <p>Enter your WiFi credentials to connect</p>
    
    <form id="wifiForm" action="/connect" method="POST">
      <div class="form-group">
        <label for="ssid">Network SSID</label>
        <input type="text" id="ssid" name="ssid" placeholder="Your WiFi network name" required>
      </div>
      
      <div class="form-group">
        <label for="password">Password</label>
        <input type="password" id="password" name="password" placeholder="Your WiFi password">
      </div>
      
      <button type="submit" class="btn">Connect</button>
    </form>
    
    <div id="statusMessage" class="status"></div>
  </div>

  <script>
    document.getElementById('wifiForm').addEventListener('submit', function(e) {
      e.preventDefault();
      
      const form = e.target;
      const formData = new FormData(form);
      const statusDiv = document.getElementById('statusMessage');
      
      fetch(form.action, {
        method: 'POST',
        body: new URLSearchParams(formData),
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        }
      })
      .then(response => {
        if (response.ok) {
          return response.text();
        }
        throw new Error('Network response was not ok.');
      })
      .then(text => {
        statusDiv.style.display = 'block';
        statusDiv.className = 'status success';
        statusDiv.innerHTML = '<h3>Success!</h3><p>Connecting to WiFi network... Please wait.</p>';
        
        // Show connection progress
        setTimeout(() => {
          statusDiv.innerHTML += '<p>If connection fails, the setup page will reappear.</p>';
        }, 2000);
      })
      .catch(error => {
        statusDiv.style.display = 'block';
        statusDiv.className = 'status error';
        statusDiv.innerHTML = '<h3>Error!</h3><p>Failed to submit credentials. Please try again.</p>';
        console.error('Error:', error);
      });
    });
  </script>
</body>
</html>
)rawliteral";

// --- Function to handle root page ("/") ---
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// --- Function to handle WiFi Connect POST form ---
void handleConnect() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    wifiSSID = server.arg("ssid");
    wifiPassword = server.arg("password");

    server.send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <style>
          body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #ff6b6b, #feca57, #48dbfb);
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            color: #2d3436;
          }
          .message {
            background: white;
            padding: 2rem;
            border-radius: 15px;
            text-align: center;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
            max-width: 500px;
          }
          h1 {
            color: #2d3436;
          }
          .spinner {
            border: 5px solid #f3f3f3;
            border-top: 5px solid #48dbfb;
            border-radius: 50%;
            width: 50px;
            height: 50px;
            animation: spin 1s linear infinite;
            margin: 20px auto;
          }
          @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
          }
        </style>
      </head>
      <body>
        <div class="message">
          <h1>Connecting to WiFi...</h1>
          <div class="spinner"></div>
          <p>Please wait while we attempt to connect to <strong>)rawliteral" + wifiSSID + R"rawliteral(</strong>.</p>
          <p>This page will close automatically when connected.</p>
        </div>
      </body>
      </html>
    )rawliteral");

    delay(1000);
    connectToWiFi();
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// --- Function to Connect to WiFi Network ---
void connectToWiFi() {
  WiFi.softAPdisconnect(true); // Stop Access Point

  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str()); // Connect to WiFi

  Serial.print("Connecting to WiFi");
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to Connect. Restarting AP mode.");
    startAP(); // Restart AP mode if failed
  }
}

// --- Function to Start Access Point and Server ---
void startAP() {
  Serial.println("Starting Access Point...");
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);

  server.begin();
  Serial.println("Web Server Started.");
}

// --- Function to Initialize WiFi Configuration ---
void initWiFi() {
  Serial.println("Initializing WiFi Setup...");
  startAP();
}

// --- Function to handle client requests (should be called inside loop) ---
void handleWiFiClient() {
  server.handleClient();
}

#endif // CONFIG_H