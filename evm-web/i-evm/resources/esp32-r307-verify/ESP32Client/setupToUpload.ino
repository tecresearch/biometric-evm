#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Adafruit_Fingerprint.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Fingerprint Sensor Configuration (Using Serial2)
#define FINGERPRINT_RX 16  // ESP32 RX2 (Connect to sensor TX)
#define FINGERPRINT_TX 17  // ESP32 TX2 (Connect to sensor RX)
HardwareSerial fingerSerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

#define ledpin_s 32
#define ledpin_p 33
// WiFi Configuration
const char* ssid = "TCA@Admin";
const char* password = "Shivam@9211";

// WebSocket Server
WebSocketsServer webSocket = WebSocketsServer(81);

// System States
enum SystemState {
  STATE_DISCONNECTED,
  STATE_CONNECTED,
  STATE_ENROLLING,
  STATE_MATCHING,
  STATE_ENROLL_SUCCESS,
  STATE_MATCH_SUCCESS
};
SystemState currentState = STATE_DISCONNECTED;

void setup() {
  Serial.begin(115200);
    pinMode(ledpin_s,OUTPUT);
    pinMode(ledpin_p,OUTPUT);
    digitalWrite(ledpin_s,LOW);
    digitalWrite(ledpin_p,LOW);
  // Initialize components
  initializeOLED();
  initializeFingerprintSensor();
  connectToWiFi();
  
  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  updateDisplay();
}

void loop() {
  webSocket.loop();
  
  // Sensor connection watchdog
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 2000) {
    lastCheck = millis();
    if (!finger.verifyPassword()) {
      currentState = STATE_DISCONNECTED;
      updateDisplay();
      webSocket.broadcastTXT("Sensor disconnected");
    }
  }
  
  handleFingerprintOperations();
}

// OLED Initialization
void initializeOLED() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
}


// Fingerprint Operations
void initializeFingerprintSensor() {
  fingerSerial.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing Sensor...");
  display.display();
  
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    currentState = STATE_CONNECTED;
    Serial.println("Sensor found");
    Serial.print("Capacity: "); Serial.println(finger.capacity);
  } else {
    currentState = STATE_DISCONNECTED;
    Serial.println("Sensor not found missing wiring");
  }
  updateDisplay();
}

// WiFi Connection
void connectToWiFi() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to WiFi");
  display.println(ssid);
  display.display();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  
  display.clearDisplay();
  display.println("WiFi connected");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
}

// WebSocket Event Handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        sendStateUpdate(num);
      }
      break;
    case WStype_TEXT:
      handleWebSocketCommand((char*)payload);
      break;
  }
}

// Command Handler
void handleWebSocketCommand(char* command) {
  if (strcmp(command, "enroll") == 0 && currentState == STATE_CONNECTED) {
    currentState = STATE_ENROLLING;
    webSocket.broadcastTXT("Enrollment started");
  } 
  else if (strcmp(command, "match") == 0 && currentState == STATE_CONNECTED) {
    currentState = STATE_MATCHING;
    webSocket.broadcastTXT("Matching started");
  }
  updateDisplay();
}

// State Update to Client
void sendStateUpdate(uint8_t clientNum) {
  String stateMsg = "state:";
  switch(currentState) {
    case STATE_DISCONNECTED: stateMsg += "disconnected"; break;
    case STATE_CONNECTED: stateMsg += "connected"; break;
    case STATE_ENROLLING: stateMsg += "enrolling"; break;
    case STATE_MATCHING: stateMsg += "matching"; break;
    case STATE_ENROLL_SUCCESS: stateMsg += "enroll_success"; break;
    case STATE_MATCH_SUCCESS: stateMsg += "match_success"; break;
  }
  webSocket.sendTXT(clientNum, stateMsg);
}

// OLED Display Update
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  switch(currentState) {
    case STATE_DISCONNECTED:
      display.println("Sensor Disconnected");
      display.println("Check Wiring");
      break;
    case STATE_CONNECTED:
      display.println("Web Enabled ");
      display.println("Fingerprint EVM");
      display.println("_______________");
      display.println("System Ready");
      display.print("Stored:"); display.println(finger.templateCount);
      display.print("IP:"); display.println(WiFi.localIP());
      display.println("Admin:Mr.Brijesh ");
      
      break;
    case STATE_ENROLLING:
      display.println("Enrollment Mode");
      display.println("Place Finger");
      digitalWrite(ledpin_s,HIGH);

      break;
    case STATE_MATCHING:
      display.println("Matching Mode");
      display.println("Place Finger");
      digitalWrite(ledpin_p,HIGH);
      break;
    case STATE_ENROLL_SUCCESS:
      display.println("Enrollment Success!");
      display.print("ID: "); display.println(finger.fingerID);
      break;
    case STATE_MATCH_SUCCESS:
      display.println("Match Found!");
      display.print("ID: "); display.println(finger.fingerID);
      break;
  }
  display.display();
}


void handleFingerprintOperations() {
  static uint8_t enrollId = 1;
  int fingerId = -1;
  
  switch(currentState) {
    case STATE_ENROLLING:
      if (enrollFingerprint(enrollId)) {
        currentState = STATE_ENROLL_SUCCESS;
        webSocket.broadcastTXT("Enrolled ID: " + String(enrollId));
        enrollId = (enrollId % finger.capacity) + 1;
      }
      break;
      
    case STATE_MATCHING:
      fingerId = getFingerprintID();
      if (fingerId != -1) {
        currentState = STATE_MATCH_SUCCESS;
        webSocket.broadcastTXT("Matched ID: " + String(fingerId));
      }
      break;
      
    case STATE_ENROLL_SUCCESS:
    case STATE_MATCH_SUCCESS:
      delay(3000);
      currentState = STATE_CONNECTED;
      break;
  }
  
  updateDisplay();
}

// Enrollment Function
uint8_t enrollFingerprint(uint8_t id) {
  int p = -1;
  
  // First finger press
  display.clearDisplay();
  display.println("Enroll ID: " + String(id));
  display.println("Place finger");
  display.display();
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) continue;
    if (p != FINGERPRINT_OK) return false;
  }

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) return false;

  // Remove finger
  display.println("Remove finger");
  display.display();
  while (finger.getImage() != FINGERPRINT_NOFINGER) delay(500);

  // Second finger press
  display.println("Same finger again");
  display.display();
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) continue;
    if (p != FINGERPRINT_OK) return false;
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) return false;

  // Create and store model
  p = finger.createModel();
  if (p != FINGERPRINT_OK) return false;

  p = finger.storeModel(id);
  return (p == FINGERPRINT_OK);
}

// Matching Function
int getFingerprintID() {
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  return (p == FINGERPRINT_OK) ? finger.fingerID : -1;
}