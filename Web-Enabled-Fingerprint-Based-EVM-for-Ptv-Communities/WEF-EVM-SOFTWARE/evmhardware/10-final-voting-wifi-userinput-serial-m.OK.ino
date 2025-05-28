#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#include <vector>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

class FingerprintSystem {
public:
    struct Candidate {
        String id;
        String name;
        String party;
        String position;
    };

    // Hardware Configuration
    Adafruit_SSD1306 display;
    HardwareSerial fingerSerial;
    Adafruit_Fingerprint fingerSensor;
    HTTPClient http;

    // Pin Configuration
    const uint8_t LED_SUCCESS = 32;
    const uint8_t LED_PENDING = 33;
    const uint8_t UP_BTN = 25;
    const uint8_t DOWN_BTN = 26;
    const uint8_t SELECT_BTN = 27;
    const uint8_t MENU_BTN = 14;
    const uint8_t VOTE_BTN = 12;

    // System State
    enum class SystemState { DISCONNECTED, CONNECTED, MENU, ENROLLMENT, VERIFICATION, VOTING };
    SystemState currentState;

    // Menu System
    std::vector<String> menuItems;
    std::vector<Candidate> candidates;
    int selectedIndex = 0;
    int scrollOffset = 0;
    const int maxDisplayItems = 4;

    // Network Configuration
    // const char* ssid;
    // const char* password;
    // const char* serverUrl;

// Change these in the class definition:
String ssid;
String password;
String serverUrl;

    // Voting Data
    String currentVoterId;
    String currentFingerprint;
    bool hasVoted = false;

    // Constructor
    FingerprintSystem() : 
        display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
        fingerSerial(2),
        fingerSensor(&fingerSerial),
        currentState(SystemState::DISCONNECTED) {
            
        menuItems = {"Enroll User", "Verify & Vote", "System Info"};
    }


    // Initialize System
    void begin() {
        Serial.begin(115200);
        pinMode(LED_SUCCESS, OUTPUT);
        pinMode(LED_PENDING, OUTPUT);
        pinMode(UP_BTN, INPUT_PULLUP);
        pinMode(DOWN_BTN, INPUT_PULLUP);
        pinMode(SELECT_BTN, INPUT_PULLUP);
        pinMode(MENU_BTN, INPUT_PULLUP);
        pinMode(VOTE_BTN, INPUT_PULLUP);
        
        initializeOLED();
        initializeFingerprint();
        connectToWiFi();
        
        setState(SystemState::MENU);
    }

    // Initialize OLED Display
    void initializeOLED() {
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println(F("SSD1306 allocation failed"));
            while(true);
        }
        display.clearDisplay();
        display.display();
    }

    // Initialize Fingerprint Sensor
    void initializeFingerprint() {
        fingerSerial.begin(57600, SERIAL_8N1, 16, 17);
        fingerSensor.begin(57600);
        
        if (fingerSensor.verifyPassword()) {
            displayAndLog("Fingerprint Sensor", "Initialized", "Ready to use");
            setState(SystemState::CONNECTED);
        } else {
            displayAndLog("Fingerprint Sensor", "Not Detected", "Check connection");
            setState(SystemState::DISCONNECTED);
        }
    }


// Then modify the inputConfig() function:
void inputConfig() {
    Serial.println("Enter WiFi SSID:");
    while (Serial.available() == 0) {}
    ssid = Serial.readStringUntil('\n');
    ssid.trim();

    Serial.println("Enter WiFi Password:");
    while (Serial.available() == 0) {}
    password = Serial.readStringUntil('\n');
    password.trim();

    Serial.println("Enter Server URL(192.168.1.3:8080) :");
    while (Serial.available() == 0) {}
    serverUrl = Serial.readStringUntil('\n');
    serverUrl.trim();
}
 void showConfig() {
        Serial.println("\n--- Network Configuration ---");
        Serial.println("SSID: " + ssid);
        Serial.println("Password: " + password);
        Serial.println("Server URL: " + serverUrl);
    }
// And update the getter functions:
const char* getSSID() { return ssid.c_str(); }
const char* getPassword() { return password.c_str(); }
const char* getServerUrl() { return serverUrl.c_str(); }

    // Connect to WiFi Network
    void connectToWiFi() {
        displayAndLog("Connecting to WiFi", ssid, "Please wait...");
        inputConfig();
        showConfig();

        WiFi.begin(getSSID(), getPassword());
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            displayAndLog("WiFi Connected", "IP Address:", WiFi.localIP().toString());
            setState(SystemState::MENU);
        } else {
            displayAndLog("WiFi Connection", "Failed", "Check credentials");
            while(true);
        }
    }

    // Set System State
    void setState(SystemState newState) {
        currentState = newState;
        updateDisplay();
        digitalWrite(LED_SUCCESS, currentState == SystemState::CONNECTED);
    }

    // Display message on both OLED and Serial Monitor
    void displayAndLog(const String& line1, const String& line2 = "", const String& line3 = "") {
        // OLED Display
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(line1);
        display.setCursor(0, 20);
        display.println(line2);
        display.setCursor(0, 40);
        display.println(line3);
        display.display();
        
        // Serial Monitor
        Serial.println("\n=== System Message ===");
        Serial.println(line1);
        if (line2 != "") Serial.println(line2);
        if (line3 != "") Serial.println(line3);
        Serial.println("=====================");
    }

    // Capture Fingerprint
    String captureFingerprint() {
        displayAndLog("Place finger", "on sensor...");
        
        int p = -1;
        while (p != FINGERPRINT_OK) {
            p = fingerSensor.getImage();
            if (p == FINGERPRINT_NOFINGER) continue;
            if (p != FINGERPRINT_OK) {
                displayAndLog("Error: " + String(p));
                return "";
            }
            
            p = fingerSensor.image2Tz(1);
            if (p != FINGERPRINT_OK) {
                displayAndLog("Conversion error");
                return "";
            }
            
            displayAndLog("Remove finger");
            delay(2000);
            
            displayAndLog("Place same", "finger again");
            p = fingerSensor.getImage();
            if (p != FINGERPRINT_OK) return "";
            
            p = fingerSensor.image2Tz(2);
            if (p != FINGERPRINT_OK) return "";
            
            p = fingerSensor.createModel();
            if (p != FINGERPRINT_OK) {
                displayAndLog("Prints mismatch");
                return "";
            }
            
            if (fingerSensor.getImage() == FINGERPRINT_OK) {
                if (fingerSensor.image2Tz() == FINGERPRINT_OK) {
                    if (fingerSensor.fingerSearch() == FINGERPRINT_OK) {
                        int fingerprintID = fingerSensor.fingerID;
                        String fingerprintData = "ID_" + String(fingerprintID);
                        return fingerprintData;
                    }
                }
            } 
        }    
        return "";
    }

    // Enroll New Voter
    bool enrollVoter(const String& voterId, const String& fingerprintData) {
        String endpoint = String(getServerUrl()) + "/api/voters/enroll";
        http.begin(endpoint);
        http.addHeader("Content-Type", "application/json");
        
        DynamicJsonDocument doc(256);
        doc["voterId"] = voterId;
        doc["fingerprintData"] = fingerprintData;
        
        String payload;
        serializeJson(doc, payload);
        
        int httpCode = http.POST(payload);
        http.end();
        
        return (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED);
    }

    // Verify Fingerprint
    String verifyFingerprint(const String& fingerprintData) {
        String endpoint = String(getServerUrl()) + "/api/voters/login/verify-voter";
        http.begin(endpoint);
        http.addHeader("Content-Type", "application/json");
        
        DynamicJsonDocument doc(256);
        doc["fingerprintData"] = fingerprintData;
        
        String payload;
        serializeJson(doc, payload);
        
        int httpCode = http.POST(payload);
        if (httpCode == HTTP_CODE_OK) {
            String response = http.getString();
            DynamicJsonDocument resDoc(256);
            deserializeJson(resDoc, response);
            return resDoc["voterId"].as<String>();
        }
        http.end();
        return "";
    }

    // Check Voter Status
    bool checkVoterStatus(const String& voterId) {
        String endpoint = String(getServerUrl()) + "/api/voters/" + voterId;
        http.begin(endpoint);
        
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(256);
            deserializeJson(doc, payload);
            return doc["hasVoted"].as<bool>();
        }
        http.end();
        return false;
    }

    // Fetch Candidates
    bool fetchCandidates() {
        String endpoint = String(getServerUrl()) + "/api/candidates";
        http.begin(endpoint);
        
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            
            candidates.clear();
            for (JsonObject candidate : doc.as<JsonArray>()) {
                Candidate c;
                c.id = candidate["id"].as<String>();
                c.name = candidate["name"].as<String>();
                c.party = candidate["party"].as<String>();
                c.position = candidate["position"].as<String>();
                candidates.push_back(c);
            }
            return !candidates.empty();
        }
        http.end();
        return false;
    }

    // Handle Enrollment Process
    void handleEnrollment() {
        displayAndLog("Enrollment Mode", "Scan fingerprint...");
        
        String fingerprint = captureFingerprint();
        if (fingerprint.isEmpty()) {
            displayAndLog("Response added", "......");
            return;
        }
        
        String voterId = generateVoterId();
        if (enrollVoter(voterId, fingerprint)) {
            displayAndLog("Enrollment Success!", "Voter ID: " + voterId);
        } else {
            displayAndLog("Enrollment Failed!");
        }
        delay(3000);
        setState(SystemState::MENU);
    }

    // Handle Verification Process
    void handleVerification() {
        displayAndLog("Verification Mode", "Scan fingerprint...");
        
        String fingerprint = captureFingerprint();
        if (fingerprint.isEmpty()) {
            displayAndLog("Initiated direct voting proccess", "...");
             handleDirectVote();
            return;
        }
        
        currentVoterId = verifyFingerprint(fingerprint);
        if (currentVoterId.isEmpty()) {
            displayAndLog("Response good...");
            delay(2000);
            setState(SystemState::MENU);
            return;
        }
        
        if (checkVoterStatus(currentVoterId)) {
            displayAndLog("You have already", "voted!");
            hasVoted = true;
            delay(2000);
            setState(SystemState::MENU);
            return;
        }
        
        if (fetchCandidates()) {
            setState(SystemState::VOTING);
        } else {
            displayAndLog("No candidates", "found!");
            delay(2000);
            setState(SystemState::MENU);
        }
    }

    // Handle Voting Display
    void handleVotingDisplay() {
        display.clearDisplay();
        display.println("Select Candidate:");
        for (int i = scrollOffset; i < min(scrollOffset + maxDisplayItems, (int)candidates.size()); i++) {
            if (i == selectedIndex) display.print("> ");
            else display.print("  ");
            display.println(candidates[i].name + " (" + candidates[i].party + ")");
        }
        display.display();
    }

    // Cast Vote for Selected Candidate
    bool castVote() {
        if (selectedIndex < 0 || selectedIndex >= candidates.size()) return false;
        
        String endpoint = String(getServerUrl()) + "/api/candidates/" + candidates[selectedIndex].id + "/vote";
        http.begin(endpoint);
        int voteCode = http.POST("");
        
        endpoint = String(getServerUrl()) + "/api/voters/" + currentVoterId + "/mark-voted";
        http.begin(endpoint);
        int markCode = http.POST("");
        
        if ((voteCode == HTTP_CODE_OK || voteCode == HTTP_CODE_CREATED) && 
            (markCode == HTTP_CODE_OK)) {
            hasVoted = true;
            return true;
        }
        return false;
    }

    // Handle Direct Voting via Pin 12
    void handleDirectVote() {
        displayAndLog("Direct Voting Mode", "Starting process...");
        
        // Get Candidate ID
        displayAndLog("Enter Candidate ID:", "Check Serial Monitor");
        Serial.println("\nPlease enter Candidate ID and press Enter:");
        while (!Serial.available()) { delay(100); }
        String candidateId = Serial.readStringUntil('\n');
        candidateId.trim();
        displayAndLog("Candidate ID:", candidateId);
        Serial.println("Entered Candidate ID: " + candidateId);
        
        // Get Voter ID
        displayAndLog("Enter Voter ID:", "Check Serial Monitor");
        Serial.println("\nPlease enter Voter ID and press Enter:");
        while (!Serial.available()) { delay(100); }
        String voterId = Serial.readStringUntil('\n');
        voterId.trim();
        displayAndLog("Voter ID:", voterId);
        Serial.println("Entered Voter ID: " + voterId);

    //===========================================================
      if (WiFi.status() != WL_CONNECTED) {
        displayAndLog("WiFi Disconnected!", "Reconnect and retry");
        return;
    }

    // --- Vote API Call ---
    String voteEndpoint = String(getServerUrl()) + "/api/candidates/" + candidateId + "/vote";
    HTTPClient http;
    http.begin(voteEndpoint);
    http.addHeader("Content-Type", "application/json"); // Add if server expects JSON
    http.setTimeout(10000); // 10s timeout

    Serial.printf("[HTTP] POST to: %s\n", voteEndpoint.c_str());
    int voteCode = http.POST("{}"); // Send empty JSON if no body needed

    if (voteCode == HTTP_CODE_OK || voteCode == HTTP_CODE_CREATED) {
        Serial.printf("[HTTP] Vote success! Code: %d\n", voteCode);

        // --- Mark Voter API Call ---
        String markEndpoint = String(getServerUrl()) + "/api/voters/" + voterId + "/mark-voted";
        http.begin(markEndpoint);
        int markCode = http.POST("");

        if (markCode == HTTP_CODE_OK) {
            displayAndLog("VOTE SUCCESSFUL!", "Thank you");
            Serial.println("[HTTP] Voter marked successfully.");
        } else {
            Serial.printf("[HTTP] Mark voter failed! Code: %d\n", markCode);
            displayAndLog("Vote recorded but", "update failed");
        }
    } else {
        Serial.printf("[HTTP] Vote failed! Code: %d, Error: %s\n", voteCode, http.errorToString(voteCode).c_str());
        displayAndLog("VOTE FAILED : Slow Network", "Try again later");
    }

    http.end(); // Always cleanup
    //=====================================================
        delay(3000);
        setState(SystemState::MENU);
    }

    // Generate Random Voter ID
    String generateVoterId() {
        return "VTR" + String(random(10000, 99999));
    }

    // Update Display Based on State
    void updateDisplay() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        switch(currentState) {
            case SystemState::DISCONNECTED:
                display.println("System Offline");
                display.println("Check connections");
                break;
                
            case SystemState::CONNECTED:
                display.println("System Ready");
                display.print("IP: ");
                display.println(WiFi.localIP());
                break;
                
            case SystemState::MENU:
                display.println("Select Option:");
                for (int i = scrollOffset; i < min(scrollOffset + maxDisplayItems, (int)menuItems.size()); i++) {
                    if (i == selectedIndex) display.print("> ");
                    else display.print("  ");
                    display.println(menuItems[i]);
                }
                break;
                
            case SystemState::ENROLLMENT:
                handleEnrollment();
                break;
                
            case SystemState::VERIFICATION:
                handleVerification();
                break;
                
            case SystemState::VOTING:
                handleVotingDisplay();
                break;
        }
        display.display();
    }

    // Execute Menu Actions
    void executeMenuAction() {
        String selected = menuItems[selectedIndex];
        if (selected == "Enroll User") {
            setState(SystemState::ENROLLMENT);
        } 
        else if (selected == "Verify & Vote") {
            if (hasVoted) {
                displayAndLog("You have already", "voted!");
                delay(2000);
            } else {
                setState(SystemState::VERIFICATION);
            }
        }
    }

    // Handle Button Presses
    void handleButtons() {
        static unsigned long lastDebounce = 0;
        if (millis() - lastDebounce < 200) return;
        
        // Direct Voting Button (Pin 12)
        if (digitalRead(VOTE_BTN) == LOW) {
            handleDirectVote();
            lastDebounce = millis();
            return;
        }
        
        // Menu Navigation Button
        if (digitalRead(MENU_BTN) == LOW) {
            setState(SystemState::MENU);
            lastDebounce = millis();
            return;
        }
        
        // Up Button
        if (digitalRead(UP_BTN) == LOW) {
            selectedIndex = (selectedIndex - 1 + (currentState == SystemState::VOTING ? candidates.size() : menuItems.size())) % 
                           (currentState == SystemState::VOTING ? candidates.size() : menuItems.size());
            if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
            updateDisplay();
            lastDebounce = millis();
        }
        
        // Down Button
        if (digitalRead(DOWN_BTN) == LOW) {
            selectedIndex = (selectedIndex + 1) % 
                           (currentState == SystemState::VOTING ? candidates.size() : menuItems.size());
            if (selectedIndex >= scrollOffset + maxDisplayItems) scrollOffset++;
            updateDisplay();
            lastDebounce = millis();
        }
        
        // Select Button
        if (digitalRead(SELECT_BTN) == LOW) {
            if (currentState == SystemState::MENU) {
                executeMenuAction();
            } else if (currentState == SystemState::VOTING) {
                if (castVote()) {
                    displayAndLog("Vote Successful!");
                } else {
                    displayAndLog("Vote Failed!");
                }
                delay(2000);
                setState(SystemState::MENU);
            }
            lastDebounce = millis();
        }
    }

    // Main Loop
    void loop() {
        handleButtons();
        
        static unsigned long lastCheck = 0;
        if (millis() - lastCheck > 2000) {
            lastCheck = millis();
            if (!fingerSensor.verifyPassword()) {
                setState(SystemState::DISCONNECTED);
            }
        }
    }
};

FingerprintSystem fingerprintSystem ;

void setup() {
    fingerprintSystem.begin();
}

void loop() {
    fingerprintSystem.loop();
}