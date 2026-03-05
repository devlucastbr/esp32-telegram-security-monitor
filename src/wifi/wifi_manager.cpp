#include "wifi_manager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
}

void WiFiManager::connect() {
    WiFi.begin(ssid, password);

    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado!");
}