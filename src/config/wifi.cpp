#include "wifi.h"
#include "display.h"
#include "../core/text.h"
#include <Arduino.h>

// WiFi credentials
const char* WIFI_SSID = "Moon-iOT";
const char* WIFI_PASSWORD = "moon_iot_182838";

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.println();
    Serial.print("Connecting to WiFi");
    text("Connecting to WiFi...", 0, 0, 1);
    
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 30) {
        delay(500);
        Serial.print(".");
        attempt++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Show connection success
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("WiFi Connected!");
        display.setCursor(0, 16);
        display.println("IP:");
        display.setCursor(0, 28);
        display.println(WiFi.localIP());
        display.display();
        delay(2000);
    } else {
        Serial.println();
        Serial.println("WiFi connection failed!");
        text("WiFi Failed!", 0, 0, 1);
        delay(2000);
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String getIPAddress() {
    return WiFi.localIP().toString();
}

