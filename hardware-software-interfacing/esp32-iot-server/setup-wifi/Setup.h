#ifndef SETUP_H
#define SETUP_H

#include "WifiConfigPage.h" // Include reusable WiFi configuration

// --- Setup function ---
void setupAppConfig() {
  Serial.begin(115200);  // Begin serial communication at 115200 baud rate
  delay(1000);           // Delay for 1 second to allow for setup time

  initWiFi(); // Call the function to initialize WiFi
}

// --- Loop function ---
void loopAppConfig() {
  handleWiFiClient(); // Continuously handle incoming HTTP client connections
}

#endif // SETUP_H
