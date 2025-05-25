#include <WiFi.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char* ssid = "TCA@Admin";
const char* password = "Shivam@9211";

// Define LED pin
#define ledPin 32

// WebSocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

bool ledState = false;
void setup() {
  // Start serial communication
  Serial.begin(115200);
   // Initialize the LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Start with LED off

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  
  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started");
}

void loop() {

  webSocket.loop();
                   
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        // Send current LED state to newly connected client
        webSocket.sendTXT(num, ledState ? "on" : "off");
      }
      break;
      
    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", num, payload);
      
      if (strcmp((char*)payload, "toggle") == 0) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW);
        
        // Broadcast new state to all connected clients
        webSocket.broadcastTXT(ledState ? "on" : "off");
        Serial.printf("LED toggled to %s\n", ledState ? "ON" : "OFF");
      }
      break;
      
    case WStype_ERROR:
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      // Handle other events if needed
      break;
  }
}
