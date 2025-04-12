#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Create a software serial port
SoftwareSerial mySerial(2, 3);  // RX, TX

// Initialize fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
    Serial.begin(9600);       // Serial Monitor
    mySerial.begin(57600);    // Fingerprint Module Default Baud Rate

    Serial.println("Initializing Fingerprint Module...");
    
    if (finger.verifyPassword()) {
        Serial.println("Fingerprint sensor found!");
    } else {
        Serial.println("No fingerprint sensor detected!");
        while (1);
    }
}

void loop() {
    Serial.println("Place your finger...");
    getFingerprintID();
    delay(2000);
}

void getFingerprintID() {
    int result = finger.getImage();
    if (result != FINGERPRINT_OK) return;

    result = finger.image2Tz();
    if (result != FINGERPRINT_OK) return;

    result = finger.fingerFastSearch();
    if (result == FINGERPRINT_OK) {
        Serial.print("Fingerprint ID: ");
        Serial.println(finger.fingerID);
    } else {
        Serial.println("No match found.");
    }
}
